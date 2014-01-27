#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "network.h"
#include "buffer.h"
#include "dy_char.h"

#define KEY_SIZE 16
#define COMMAND_SIZE 16
#define NUM_SIZE 16
thread_t threads[WORKER_NUM];

int worker_index=0;

static void *worker_loop(void *arg);

static connection_t* init_connection();

static void reset_read_context(read_context_t *rc);

static int no_block_write(int fd,buffer_t *wbuf);

extern int parse_command(connection_t *conn);

extern int process_command(connection_t *conn);

void start_workers(){
  int i=0;
  for(;i<WORKER_NUM;i++){
    threads[i].pipe_channel=(pipe_channel_t*)malloc(sizeof(pipe_channel_t));
    pipe_channel_t *p=threads[i].pipe_channel;
    threads[i].queue=init_queue();
    if(p==NULL){
      printf("create pipe channel error\n");
      exit(0);
    }
    int fd[2];
    if(pipe(fd)<0){
      printf("create pipe error\n");
      exit(0);
    }
    p->masterfd=fd[1];
    p->workerfd=fd[0];
    pthread_t tid=0;
    pthread_create(&tid,NULL,worker_loop,threads+i);
  }
}

void *worker_loop(void *arg){
  thread_t* t=(thread_t *)arg;
  event_context_t ec;
  int notify_fd=t->pipe_channel->workerfd;
  event_operation.init_event(&ec);
  ec.worker_fd=notify_fd;
  ec.queue=t->queue;
  event_operation.register_event(notify_fd,READ,&ec,t);
  while(1){
    event_operation.process_event(&ec);    
  }
}


int handle_write(connection_t *conn){
  write_context_t *wc=conn->wc;
  char *target=wc->w_data;
  buffer_t *wbuf=conn->wbuf;
  int result;
  //每次写数据都是先发到w_queue中，写的时候先pop
  while(1){
    if(target==NULL){
      target=pop(wc->w_queue);
    } 
    //如果w_data和队列中没有，表示没有写的数据了
    if(target==NULL){
      //如果还有数据没写完，继续写
      if(has_remaining(wbuf)){
        return no_block_write(conn->fd,wbuf);
      }
      return OK;
    }
    //计算最多容量
    int avilable=wbuf->size-wbuf->limit;
    //计算数据量
    int copy=strlen(target)-wc->w_index;
    if(avilable<copy){
      copy=avilable;
    }
    //开始copy
    memcpy(wbuf->data+wbuf->limit,target+wc->w_index,copy);
    //更新指针
    wc->w_index+=copy;
    wbuf->limit+=copy;
    //如果wbuf还有空间，先不write，继续填充wbuf
    if(has_space(wbuf)){
      free(target);
      target=NULL;
      wc->w_index=0;
    }else{
      //wbuf满了，开始write
      int result=no_block_write(conn->fd,wbuf);
      //如果没写完，返回等待写事件，否则继续填充wbuf
      if(!result){
        return ENOUGH;
      }
    }
  }
}

static int no_block_write(int fd,buffer_t *wbuf){
   int count=write(fd,wbuf->data+wbuf->current,wbuf->limit-wbuf->current);
   wbuf->current+=count;
   compact(wbuf);
      //如果count比预期要小，说明不能再写了
   if(count<wbuf->limit-wbuf->current){
        return ENOUGH;
   } 
   return OK;
}

void handle_read(connection_t *conn){
  buffer_t *rbuf=conn->rbuf;
  while(1){
    int count=read(conn->fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
    if(count<=0){
      printf("read error\n");
      return;
    }
    int read_all=rbuf->size-rbuf->limit;
    rbuf->limit=rbuf->current+count;
    int result;
    //表示还有数据需要处理
    while(has_remaining(rbuf)){
      //command还未接收处理完
      if(conn->rc->read_process<READ_COMMAND_END){
        //接收处理command
        result=parse_command(conn);
        //表示command行没有接收全部，read没有read完全
        if(result==AGAIN){
          reset(rbuf);
          break;
        }
      }
      //处理command操作
      result=process_command(conn);
      //不管什么结果都收紧buf，去掉已经处理完成的数据，重置指针
      if(result==AGAIN){
        reset(rbuf);
        break;
      }else{
        reset_read_context(conn->rc);
        //compact(rbuf);
      }   
    }
    reset(rbuf);
    //如果本次read接收的数据小于预期，那么说明不用再次read了，os缓冲已经没有数据了，否则需要再次read，继续取数据
    if(count<read_all){
      return ;
    }
  }
}

void handle_notify(int fd,event_context_t *ec){
  char notify_buf[1];
  read(fd,notify_buf,1);
  switch(notify_buf[0]){
    case 'c':
      {
        connection_t *co=init_connection();
        co->ec=ec;
        co->fd=*(int *)pop(ec->queue);
        event_operation.register_event(co->fd,READ,ec,co);
        break;
      }
  }
}

static void reset_read_context(read_context_t *rc){
  reset_char(rc->key);
  reset_char(rc->command);
  reset_char(rc->num);
  rc->last_bytes=0;
  rc->read_process=READ_COMMAND;
}

static connection_t* init_connection(){
  connection_t *co=(connection_t *)malloc(sizeof(connection_t));
  co->rbuf=alloc_buffer(READ_BUF_SIZE);
  co->wbuf=alloc_buffer(WRITE_BUF_SIZE);
  read_context_t *rc=(read_context_t *)malloc(sizeof(read_context_t));
  co->rc=rc;
  rc->read_process=READ_COMMAND;
  rc->command=init_char(COMMAND_SIZE);
  rc->key=init_char(KEY_SIZE);
  rc->num=init_char(NUM_SIZE);
  rc->last_bytes=0;
  write_context_t *wc=(write_context_t *)malloc(sizeof(write_context_t));
  co->wc=wc;
  wc->w_queue=init_queue();
  wc->w_index=0;
  return co;
}
