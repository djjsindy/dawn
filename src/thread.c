#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "connection.h"
#include "thread.h"
#include "network.h"
#include "buffer.h"
#include "dy_char.h"
#include "memory.h"
#include "my_log.h"
#include "item.h"
#include "config.h"

#define WORKER_NUM 8

static intptr_t worker_num=WORKER_NUM;

static thread_t **threads;

static intptr_t worker_index=0;

static pthread_mutex_t init_lock;

static pthread_cond_t init_cond;

static intptr_t init_count=0;

extern mem_pool_t *pool;

extern intptr_t server_sock_fd;

static void *worker_loop(void *arg);

static intptr_t no_block_write(intptr_t fd,buffer_t *wbuf);

static void init_thread(thread_t *t);

static void set_worker_num_value(char_t *value);

extern intptr_t parse_command(connection_t *conn);

extern intptr_t process_command(connection_t *conn);

extern intptr_t set_noblocking(intptr_t fd);

/**
 * 启动worker线程
 */
void start_workers(){
  intptr_t i=0;
  threads=(thread_t **)alloc_mem(pool,sizeof(thread_t *)*worker_num);
  for(;i<worker_num;i++){
    pthread_t tid=0;
    pthread_create(&tid,NULL,worker_loop,threads+i);
  }
}

/**
 * worker线程执行loop
 */
static void *worker_loop(void *arg){
  thread_t **t=(thread_t **)arg;
  *t=(thread_t *)alloc_mem(pool,sizeof(thread_t));

  //初始化线程数据
  init_thread(*t);
  thread_t *thread=*t;
  event_context_t *ec=(event_context_t *)alloc_mem(pool,sizeof(event_context_t));
  int notify_fd=thread->pipe_channel->workerfd;

  //回调网络层结构
  event_operation.init_event(ec);
  ec->worker_fd=notify_fd;
  ec->queue=thread->queue;

  //加入notifyfd监听，当主线程有连接，会通知相关的worker线程
  connection_t *conn=init_connection(notify_fd);
  event_operation.register_event(notify_fd,READ,ec,conn);

  //开始处理网络事件
  while(1){
    event_operation.process_event(ec);    
  }
}

/**
 * 初始化线程的数据，和主线程之间的管道
 */
static void init_thread(thread_t *t){
  t->pipe_channel=(pipe_channel_t*)alloc_mem(pool,sizeof(pipe_channel_t));
  if(t->pipe_channel==NULL){
    my_log(ERROR,"init thread pipe channel failed\n");
  }
  pipe_channel_t *p=t->pipe_channel;
  t->queue=init_queue();
  if(p==NULL){
    my_log(ERROR,"init queue failed\n");
  }
  int fd[2];
  if(pipe(fd)<0){
    my_log(ERROR,"init queue pipe failed\n");
  }
  p->masterfd=fd[1];
  p->workerfd=fd[0];
}


/**
 * 写事件回调函数
 */
intptr_t handle_write(connection_t *conn){
  write_context_t *wc=conn->wc;
  buffer_t *wbuf=conn->wbuf;
  intptr_t result;

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
    intptr_t avilable=wbuf->size-wbuf->limit;

    //计算数据量
    intptr_t copy=wc->w_data->c_size-wc->w_index;
    if(avilable<copy){
      copy=avilable;
    }

    //开始copy
    memcpy(wbuf->data+wbuf->limit,wc->w_data->data+wc->w_index,copy);

    //更新指针
    wc->w_index+=copy;
    wbuf->limit+=copy;

    //copy完成了，销毁item_t
    if(copy==wc->w_data->c_size){
      destroy_item(wc->w_data);     
      wc->w_data=NULL;
      wc->w_index=0;
    }

    //如果wbuf还有空间，先不write，继续填充wbuf
    if(!has_space(wbuf)){

      //wbuf满了，开始write
      intptr_t result=no_block_write(conn->fd,wbuf);

      //如果没写完，返回等待写事件，否则继续填充wbuf
      if(result==ENOUGH){
        return ENOUGH;
      }
    }
  }
}


/**
 * 非阻塞写操作
 */
static intptr_t no_block_write(intptr_t fd,buffer_t *wbuf){
  intptr_t want=wbuf->limit-wbuf->current;
  intptr_t count=write(fd,wbuf->data+wbuf->current,want);
  wbuf->current+=count;
  compact(wbuf);

  //如果count比预期要小，说明不能再写了
  if(count<want){
    return ENOUGH;
  } 
  return OK;
}

/**
 * 这里read 不能while true 的read，因为当buffer缓冲区一直有数据（command 太多），导致了没有机会write，
 * 必须等到read全部完成后，才能write，这里只read一次。一边read一边write
 */
void handle_read(connection_t *conn){
  buffer_t *rbuf=conn->rbuf;
  intptr_t count=read(conn->fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
  if(count<0){
    return;
  }else if(count==0){
    event_operation.close_event(conn->fd,conn->ec);
    cancel_connection(conn);
    return;
  }
  rbuf->limit+=count;
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
    
    //如果处理未完成
    if(result==AGAIN){
      break;
    }else{
      reset_read_context(conn->rc);
    }   
  }
  reset(rbuf);
}

/**
 * 当主线程有连接。通知worker线程去处理
 */
void handle_notify(intptr_t fd,event_context_t *ec){
  char notify_buf[1];
  read(fd,notify_buf,1);
  switch(notify_buf[0]){
    case 'c':
      {

        //获得fd
        intptr_t *i=(intptr_t *)pop(ec->queue);

        //初始化connection结构
        connection_t *co=init_connection(*i);
        co->ec=ec;
        co->events=0;

        //注册读事件
        event_operation.register_event(co->fd,READ,ec,co);
        co->events|=READ;
        free_mem(i);
        break;
      }
  }
}

/**
 * 主线程去接受连接
 */
void accept_connection(){
  struct sockaddr_in client_address;
  socklen_t address_len;
  intptr_t client_socket_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);

  //设置非阻塞模式
  if(set_noblocking(client_socket_fd)<0){
    my_log(ERROR,"set none block mode failed\n");
  }

  //选取一个worker，向管道写入通知
  char notify_buf='c';
  thread_t *t=*(threads+worker_index);
  intptr_t *i=(intptr_t *)alloc_mem(pool,sizeof(int));
  *i=client_socket_fd;

  //把连接fd写入这个线程的连接队列中
  push(t->queue,i);
  intptr_t fd=t->pipe_channel->masterfd;
  write(fd,&notify_buf,1);

  //更新轮询worker
  worker_index++;
  if(worker_index==WORKER_NUM){
    worker_index=0;
  }
}

/**
 * 关闭连接，释放相关数据
 */
void close_connection(connection_t *conn){
  event_operation.close_event(conn->fd,conn->ec);
  close(conn->fd);
  cancel_connection(conn);
}

static command_t thread_command[]={
  {"worker_num",set_worker_num_value},
  NULL
};

config_module_t thread_conf_module={
  "thread",
  thread_command
};

static void set_worker_num_value(char_t *value){
  worker_num=atoi(value->data);
}

