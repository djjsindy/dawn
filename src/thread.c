#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "network.h"
thread_t threads[WORKER_NUM];

int worker_index=0;

static void *worker_loop(void *arg);


void start_workers(){
  int i=0;
  for(;i<WORKER_NUM;i++){
    threads[i].pipe_channel=(pipe_channel_t*)malloc(sizeof(pipe_channel_t));
    threads[i].newconn=init_queue();
    pipe_channel_t *p=threads[i].pipe_channel;
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
  t->kqueuefd=ec.fd;
  event_operation.register_event(notify_fd,READ,t,t);
  while(1){
    event_operation.process_event(&ec,t);    
  }
}


void handle_write(connection_t *conn){
}

void handle_read(connection_t *conn){
  char buf[10];
  memset(buf,0,10);
  read(conn->fd,buf,10);
  puts(buf);
}

void handle_notify(int fd,thread_t *t){
  char notify_buf[1];
  read(fd,notify_buf,1);
  switch(notify_buf[0]){
    case 'c':
      {
        connection_t *co=(connection_t *)malloc(sizeof(connection_t));
        co->fd=*(int *)pop(t->newconn);
        event_operation.register_event(co->fd,READ,t,co);
        break;
      }
  }
}
