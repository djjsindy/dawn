#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "thread.h"
#include "network.h"
thread_t threads[WORKER_NUM];

static void *worker_loop(void *arg);

extern event_operation_t event_operation;

void start_workers(){
  int i=0;
  for(;i<WORKER_NUM;i++){
     threads[i].pipe_channel=(pipe_channel_t*)malloc(sizeof(pipe_channel_t*));
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
    event_operation.register_event(notify_fd,EVFILT_READ,&ec,t);
    while(1){
       event_operation.process_event(&ec);    
    }
}


void handle_write(int fd){
}

void handle_read(int fd){
    char buf[10];
    read(fd,buf,10);
    puts(buf);
}

void handle_notify(int fd,event_context_t *ec,thread_t *t){
   char *notify_buf=(char*)malloc(32);
   read(fd,notify_buf,32);
   switch(notify_buf[0]){
    case 'c':
      sscanf(notify_buf+1, "%d", &fd);
      event_operation.register_event(fd,EVFILT_READ,ec,t);
   }
   free(notify_buf);
}
