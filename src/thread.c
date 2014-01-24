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
  while(1){
    if(target==NULL){
      target=pop(wc->w_queue);
    } 
    if(target==NULL){
      return 0;
    }
    buffer_t *wbuf=conn->wbuf;
    int avilable=wbuf->size-wbuf->limit;
    int copy=sizeof(target)-wc->w_index;
    if(avilable<copy){
      copy=avilable;
    }
    memcpy(wbuf->data,wbuf->limit,copy);
    wc->w_index+=copy;
    wbuf->limit+=copy;
    if(has_space(wbuf)){
      target=NULL;
      wc->w_index=0;
    }else{
      int count=write(conn->fd,wbuf->data+wbuf->current,wbuf->limit-wbuf->current);
      wbuf->current+=count;
      compact(wbuf);
      if(count<wbuf->limit-wbuf->current){
        return 1;
      }    
    }
  }
}

void handle_read(connection_t *conn){
  buffer_t *rbuf=conn->rbuf;
  while(1){
    int count=read(conn->fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
    int read_all=rbuf->size-rbuf->limit;
    if(count==-1){
      printf("read error");
      return;
    }
    rbuf->limit=rbuf->current+count;
    int result;
    //表示还有数据需要处理
    while(has_remaining(rbuf)){
      //command还未接收处理完
      if(conn->rc->read_process!=READ_COMMAND_END){
        //接收处理command
        result=parse_command(conn);
        //表示command行没有接收全部，read没有read完全
        if(result==AGAIN){
          reset(rbuf);
          break;
        }
        //一个command处理完毕，清除connection中的状态
        else if(result==OK){
          reset_read_context(conn->rc);
          continue;
        }
      }
      //处理command操作
      result=process_command(conn);
      //不管什么结果都收紧buf，去掉已经处理完成的数据，重置指针
      compact(rbuf);
      if(result==AGAIN){
        break;
      }    
    }
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
