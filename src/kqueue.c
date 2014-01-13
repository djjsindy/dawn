#include "network.h"
#include "thread.h"
#include <sys/event.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define MAX_EVENT_COUNT 1024

static void kqueue_init_event(event_context_t *ec);

static void kqueue_register_event(int fd,int flag,event_context_t *ec,void *data);

static void kqueue_add_listen_event(event_context_t *ec);

static void kqueue_del_event(int fd,int flag);

static void kqueue_process_listen_event(event_context_t *ec);

static void kqueue_process_event(event_context_t *ec);

extern int worker_index;

char notify_buf[32];

extern int server_sock_fd;

extern thread_t threads[WORKER_NUM];

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
    perror("创建kqueue出错!\n");
    exit(1);
  }
  ec->fd=kq;
  struct kevent events[MAX_EVENT_COUNT];
  ec->events=&events;
}

static void kqueue_register_event(int fd,int flag,event_context_t *ec,void *data){
  struct kevent event;
  EV_SET(&event,fd,flag,EV_ADD,0,0,data);
  kevent(ec->fd,&event,1,NULL,0,NULL);
}

static void kqueue_add_listen_event(event_context_t *ec){
  struct kevent event;
  EV_SET(&event, ec->listen_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  kevent(ec->fd, &event, 1, NULL, 0, NULL); 
}

static void kqueue_del_event(int fd,int flag){

}

static void kqueue_process_listen_event(event_context_t *ec){
  int ret = kevent(ec->fd, NULL, 0, (struct kevent *)ec->events,MAX_EVENT_COUNT , NULL);
  if (ret < 0) {
    printf("kevent 出错!\n");
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
      *notify_buf='c';
      sprintf(notify_buf+1,"%d",client_socket_fd);
      thread_t *t=&threads[worker_index];
      int fd=t->pipe_channel->masterfd;
      write(fd,notify_buf,strlen(notify_buf));
      worker_index++;
      if(worker_index==WORKER_NUM){
        worker_index=0;
      }
    }   
  }
} 


static void kqueue_process_event(event_context_t *ec){
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  int ret = kevent(ec->fd, NULL, 0, (struct kevent *)ec->events, MAX_EVENT_COUNT, &tv);
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
    thread_t *thread=events[i].udata;
    if(sockfd==thread->pipe_channel->workerfd){
      handle_notify(sockfd,ec,thread);
    }else{
      if(events[i].filter==EVFILT_READ){
        handle_read(sockfd);
      }else if(events[i].filter==EVFILT_WRITE){
        handle_write(sockfd);
      }
    }
  }

}



