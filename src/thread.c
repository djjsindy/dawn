#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "thread.h"

thread_t threads[WORKER_NUM];

static void *worker_loop(void *arg);

static void handleIO(thread_t *thread,int count);

static void handleWrite(struct kevent *event);

static void handleRead(struct kevent *event);

static void handleNotify(struct kevent *event);

static void add_event(int fd,int16_t flag,thread_t*t);

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
    int notifyfd=t->pipe_channel->workerfd;
    int kqueuefd=kqueue();
    if(kqueuefd==-1){
      printf("create thread kqueue error");
      exit(0);
    }
    t->kqueuefd=kqueuefd;
    add_event(t->pipe_channel->workerfd,EVFILT_READ,t); 
    while(1){
        int ret = kevent(kqueuefd, NULL, 0, t->events, MAX_EVENT_COUNT, NULL);        
        if(ret==-1){
           printf("kevent error");
           exit(0);
        }else if(ret==0){
           continue;
        }
        handleIO(t,ret);
    }
}

static void add_event(int fd,int16_t flag,thread_t* t){
    struct kevent event;
    EV_SET(&event,fd,flag,EV_ADD,0,0,t);
    kevent(t->kqueuefd,&event,1,NULL,0,NULL);
}

static void handleIO(thread_t *thread,int count){
  int i=0;
  struct kevent *events=thread->events;
  for(;i<count;i++){
    int sockfd= events[i].ident; 
    int data = events[i].data;
    if(sockfd==thread->pipe_channel->workerfd){
        handleNotify(&events[i]);
    }else{
        if(events[i].filter==EVFILT_READ){
            handleRead(&events[i]);
        }else if(events[i].filter==EVFILT_WRITE){
            handleWrite(&events[i]);
        }
    }
  }
}

static void handleWrite(struct kevent *event){
}

static void handleRead(struct kevent *event){
    
    int fd=event->ident;
    char buf[10];
    read(fd,buf,10);
    puts(buf);
}

static void handleNotify(struct kevent *event){
   int fd=event->ident;
   thread_t *thread=event->udata;
   int availble=event->data;
   char *notify_buf=(char*)malloc(32);
   read(fd,notify_buf,32);
   switch(notify_buf[0]){
    case 'c':
      //int fd=0;
      sscanf(notify_buf+1, "%d", &fd);
      add_event(fd,EVFILT_READ,thread);
   }
   free(notify_buf);
}
