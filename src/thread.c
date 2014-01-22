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

static void reset_connection(connection_t *t);

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


void handle_write(connection_t *conn){
}

void handle_read(connection_t *conn){
  buffer_t *rbuf=conn->rbuf;
  int count=read(conn->fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
  rbuf->limit=rbuf->current+count;
  int result;
  while(1){
    if(conn->read_process!=READ_COMMAND_END){
      result=parse_command(conn);
      if(result==AGAIN){
        reset(rbuf);
        return;
      }
      else if(result==DATA_OK){
        reset_connection(conn);
        continue;
      }
    }
    result=process_command(conn);
    if(result==AGAIN){
      reset(rbuf);
      return;
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

static void reset_connection(connection_t *conn){
   reset_char(conn->key);
   reset_char(conn->command);
   reset_char(conn->num);
   conn->last_bytes=0;
   conn->read_process=READ_COMMAND;
}

static connection_t* init_connection(){
  connection_t *co=(connection_t *)malloc(sizeof(connection_t));
  co->rbuf=alloc_buffer(READ_BUF_SIZE);
  co->wbuf=alloc_buffer(WRITE_BUF_SIZE);
  co->read_process=READ_COMMAND;
  co->command=init_char(COMMAND_SIZE);
  co->key=init_char(KEY_SIZE);
  co->num=init_char(NUM_SIZE);
  return co;
}
