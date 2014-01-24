#include "network.h"
#include "thread.h"
#include <stdio.h>
#include <sys/event.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EVENT_COUNT 1024

static void kqueue_init_event(event_context_t *ec);

static void kqueue_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void kqueue_add_listen_event(event_context_t *ec);

static void kqueue_del_event(int fd,enum EVENT event,event_context_t *ec);

static void kqueue_process_listen_event(event_context_t *ec);

static void kqueue_process_event(event_context_t *ec);

extern int worker_index;

extern int server_sock_fd;

extern thread_t threads[WORKER_NUM];

const struct timespec ts={1,0};

extern int set_noblocking(int fd);

event_operation_t event_operation={
  kqueue_init_event,
  kqueue_register_event,
  kqueue_add_listen_event,
  kqueue_del_event,
  kqueue_process_listen_event,
  kqueue_process_event
};

static void kqueue_init_event(event_context_t *ec){ 
  int kq = kqueue();
  if (kq == -1) {
    printf("create kqueue error\n");
    exit(0);
  }
  ec->fd=kq;
  struct kevent events[MAX_EVENT_COUNT];
  ec->events=&events;
}

static void kqueue_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  struct kevent kev;
  switch(event){
    case READ:
       EV_SET(&kev,fd,EVFILT_READ,EV_ADD,0,0,data);
       break;
    case WRITE:
       EV_SET(&kev,fd,EVFILT_WRITE,EV_ADD,0,0,data);
       break;
  }
  kevent(ec->fd,&kev,1,NULL,0,NULL);
}

static void kqueue_add_listen_event(event_context_t *ec){
  struct kevent event;
  EV_SET(&event, ec->listen_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  kevent(ec->fd, &event, 1, NULL, 0, NULL); 
}
static void kqueue_del_event(int fd,enum EVENT event,event_context_t *ec){
  struct kevent kev;
  switch(event){
    case READ:
       EV_SET(&kev,fd,EVFILT_READ,EV_DELETE,0,0,NULL);
       break;
    case WRITE:
       EV_SET(&kev,fd,EVFILT_WRITE,EV_DELETE,0,0,NULL);
       break;
  }
  kevent(ec->fd,&kev,1,NULL,0,NULL);
}

static void kqueue_process_listen_event(event_context_t *ec){
  int ret = kevent(ec->fd, NULL, 0, (struct kevent *)ec->events,MAX_EVENT_COUNT , &ts);
  if (ret < 0) {
    printf("kevent create error\n");
    return;
  }else if(ret == 0){
    return;
  }
  struct kevent *events=(struct kevent *)ec->events;
  for (int i = 0; i < ret; i++) {
    struct kevent current_event = events[i];
    if(current_event.ident == ec->listen_fd){
      struct sockaddr_in client_address;
      socklen_t address_len;
      int client_socket_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);
      if(set_noblocking(client_socket_fd)<0){
         printf("set no blocking error");
         exit(0);
      }
      char notify_buf='c';
      thread_t *t=&threads[worker_index];
      push(t->queue,&client_socket_fd);
      int fd=t->pipe_channel->masterfd;
      write(fd,&notify_buf,1);
      worker_index++;
      if(worker_index==WORKER_NUM){
        worker_index=0;
      }
    }   
  }
} 


static void kqueue_process_event(event_context_t *ec){
  int ret = kevent(ec->fd, NULL, 0, (struct kevent *)ec->events, MAX_EVENT_COUNT, &ts);
  if(ret==-1){
    printf("kevent error");
    exit(0);
  }else if(ret==0){
    return;
  }
  int i=0;
  struct kevent *events=ec->events;
  for(;i<ret;i++){
    int sockfd= events[i].ident; 
    if(sockfd==ec->worker_fd){
      handle_notify(sockfd,ec);
    }else{
      connection_t *conn=(connection_t *)events[i].udata;
      if(events[i].filter==EVFILT_READ){
        handle_read(conn);
      }else if(events[i].filter==EVFILT_WRITE){
        int result=handle_write(conn);
        if(result){
          event_operation.del_event(sockfd,WRITE,ec);
        }
      }
    }
  }

}



