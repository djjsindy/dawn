#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "connection.h"
#include "thread.h"
#include "network.h"
#include "buffer.h"
#include "dy_char.h"
#include "memory.h"
#include "my_log.h"

static thread_t threads[WORKER_NUM];

static int worker_index=0;

static pthread_mutex_t init_lock;

static pthread_cond_t init_cond;

static int init_count=0;

extern mem_pool_t *pool;

extern jmp_buf exit_buf;

extern int server_sock_fd;

static void *worker_loop(void *arg);

static int no_block_write(int fd,buffer_t *wbuf);

static void init_thread(thread_t *t);

static void wait_thread_init();

static void register_thread_init();

extern int parse_command(connection_t *conn);

extern int process_command(connection_t *conn);

extern int set_noblocking(int fd);

void start_workers(){
  pthread_mutex_init(&init_lock, NULL);
  pthread_cond_init(&init_cond, NULL);
  int i=0;
  for(;i<WORKER_NUM;i++){
    pthread_t tid=0;
    pthread_create(&tid,NULL,worker_loop,threads+i);
  }
  pthread_mutex_lock(&init_lock);
  wait_thread_init();
  pthread_mutex_unlock(&init_lock);
}

static void wait_thread_init(){
  while (init_count < WORKER_NUM) {
    pthread_cond_wait(&init_cond, &init_lock);
  }
}

static void register_thread_init(){
  pthread_mutex_lock(&init_lock);
  init_count++;
  pthread_cond_signal(&init_cond);
  pthread_mutex_unlock(&init_lock);
}

static void *worker_loop(void *arg){
  thread_t* t=(thread_t *)arg;
  init_thread(t);
  event_context_t *ec=(event_context_t *)alloc_mem(pool,sizeof(event_context_t));
  int notify_fd=t->pipe_channel->workerfd;
  event_operation.init_event(ec);
  ec->worker_fd=notify_fd;
  ec->queue=t->queue;
  event_operation.register_event(notify_fd,READ,ec,t);
  register_thread_init();
  while(1){
    event_operation.process_event(ec);    
  }
}

static void init_thread(thread_t *t){
  t->pipe_channel=(pipe_channel_t*)alloc_mem(pool,sizeof(pipe_channel_t));
  if(t->pipe_channel==NULL){
    my_log(ERROR,"init thread pipe channel failed\n");
    longjmp(exit_buf,-1);
  }
  pipe_channel_t *p=t->pipe_channel;
  t->queue=init_queue();
  if(p==NULL){
    my_log(ERROR,"init queue failed\n");
    longjmp(exit_buf,-1);
  }
  int fd[2];
  if(pipe(fd)<0){
    my_log(ERROR,"init queue pipe failed\n");
    longjmp(exit_buf,-1);
  }
  p->masterfd=fd[1];
  p->workerfd=fd[0];
}

int handle_write(connection_t *conn){
  write_context_t *wc=conn->wc;
  buffer_t *wbuf=conn->wbuf;
  int result;
  //每次写数据都是先发到w_queue中，写的时候先pop
  while(1){
    if(wc->w_data==NULL){
      wc->w_data=pop(wc->w_queue);
    } 
    //如果w_data和队列中没有，表示没有写的数据了
    if(wc->w_data==NULL){
      //如果还有数据没写完，继续写
      if(has_remaining(wbuf)){
        return no_block_write(conn->fd,wbuf);
      }
      return OK;
    }
    //计算最多容量
    int avilable=wbuf->size-wbuf->limit;
    //计算数据量
    int copy=wc->w_data->c_size-wc->w_index;
    if(avilable<copy){
      copy=avilable;
    }
    //开始copy
    memcpy(wbuf->data+wbuf->limit,wc->w_data->data+wc->w_index,copy);
    //更新指针
    wc->w_index+=copy;
    wbuf->limit+=copy;
    if(copy==wc->w_data->c_size){     
      free_mem(wc->w_data->data);
      free_mem(wc->w_data);
      wc->w_data=NULL;
      wc->w_index=0;
    }

    //如果wbuf还有空间，先不write，继续填充wbuf
    if(!has_space(wbuf)){
      //wbuf满了，开始write
      int result=no_block_write(conn->fd,wbuf);
      //如果没写完，返回等待写事件，否则继续填充wbuf
      if(result==ENOUGH){
        return ENOUGH;
      }
    }
  }
}

static int no_block_write(int fd,buffer_t *wbuf){
  int want=wbuf->limit-wbuf->current;
  int count=write(fd,wbuf->data+wbuf->current,want);
  wbuf->current+=count;
  compact(wbuf);
  //如果count比预期要小，说明不能再写了
  if(count<want){
    return ENOUGH;
  } 
  return OK;
}

//这里read 不能while true 的read，因为当buffer缓冲区一直有数据（command 太多），导致了没有机会write，必须等到read全部完成后，才能write，这里只read一次。一边read一边write
void handle_read(connection_t *conn){
  buffer_t *rbuf=conn->rbuf;
  int count=read(conn->fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
  if(count<0){
    return;
  }else if(count==0){
    event_operation.close_event(conn->fd,conn->ec);
    cancel_connection(conn);
    return;
  }
  rbuf->limit=rbuf->current+count;
  int result;
        //表示还有数据需要处理
  while(has_remaining(rbuf)){
          //command还未接收处理完
    if(conn->rc->read_process==READ_COMMAND){
            //接收处理command
      result=parse_command(conn);
            //表示command行没有接收全部，read没有read完全
      if(result==AGAIN){
        break;
      }
    }
          //处理command操作
    result=process_command(conn);
          //不管什么结果都收紧buf，去掉已经处理完成的数据，重置指针
    if(result==AGAIN){
      break;
    }else{
      reset_read_context(conn->rc);
    }   
  }
  reset(rbuf);
}

void handle_notify(int fd,event_context_t *ec){
  char notify_buf[1];
  read(fd,notify_buf,1);
  switch(notify_buf[0]){
    case 'c':
      {
        connection_t *co=init_connection();
        co->ec=ec;
        int *i=(int *)pop(ec->queue);
        printf("fd %d\n",*i);
        co->fd=*i;
        event_operation.register_event(co->fd,READ,ec,co);
        free_mem(i);
        break;
      }
  }
}

void accept_connection(){
  struct sockaddr_in client_address;
  socklen_t address_len;
  int client_socket_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);
  if(set_noblocking(client_socket_fd)<0){
    my_log(ERROR,"set none block mode failed\n");
  }
  char notify_buf='c';
  thread_t *t=&threads[worker_index];
  int *i=(int *)alloc_mem(pool,sizeof(int));
  *i=client_socket_fd;
  push(t->queue,i);
  int fd=t->pipe_channel->masterfd;
  write(fd,&notify_buf,1);
  worker_index++;
  if(worker_index==WORKER_NUM){
    worker_index=0;
  }
}
