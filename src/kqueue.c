#ifdef HAVE_EVENT
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include <stdio.h>
#include <sys/event.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static void kqueue_init_event(event_context_t *ec);

static void kqueue_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void kqueue_del_event(int fd,enum EVENT event,event_context_t *ec);

static void kqueue_process_event(event_context_t *ec);

extern int server_sock_fd;

extern jmp_buf exit_buf;

const struct timespec ts={1,0};

event_operation_t event_operation={
  kqueue_init_event,
  kqueue_register_event,
  kqueue_del_event,
  kqueue_process_event
};

static void kqueue_init_event(event_context_t *ec){ 
  int kq = kqueue();
  if (kq == -1) {
    my_log(ERROR,"create kqueue error\n");
    longjmp(exit_buf,-1);
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


static void kqueue_process_event(event_context_t *ec){
  int ret = kevent(ec->fd, NULL, 0, (struct kevent *)ec->events, MAX_EVENT_COUNT, &ts);
  if(ret==-1){
    my_log(ERROR,"kqueue event error\n");
    return;
  }else if(ret==0){
    return;
  }
  int i=0;
  struct kevent *events=(struct kevent *)ec->events;
  for(;i<ret;i++){
    int sockfd= events[i].ident; 
    if(sockfd==ec->worker_fd){
      handle_notify(sockfd,ec);
    }else if(sockfd==ec->listen_fd){
      accept_connection();
    }else{
      connection_t *conn=(connection_t *)events[i].udata;
      if(events[i].filter==EVFILT_READ){
        handle_read(conn);
      }else if(events[i].filter==EVFILT_WRITE){
        int result=handle_write(conn);
        if(result==OK){
          event_operation.del_event(sockfd,WRITE,ec);
        }
      }
    }
  }
}

#endif
