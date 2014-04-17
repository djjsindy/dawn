#ifdef HAVE_POLL
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include "memory.h"
#include "list.h"
#include <stdio.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int timeout=1;

static void poll_init_event(event_context_t *ec);

static void poll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void poll_del_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void poll_process_event(event_context_t *ec);

static void poll_close_event(int fd,event_context_t *ec);

static int find_fd_slot(event_t *events,int fd);

extern int worker_index;

extern int server_sock_fd;

extern thread_t threads[WORKER_NUM];

extern int set_noblocking(int fd);

extern mem_pool_t *pool;

event_operation_t event_operation={
  poll_init_event,
  poll_register_event,
  poll_del_event,
  poll_process_event,
  poll_close_event
};

static void poll_init_event(event_context_t *ec){ 
  struct pollfd *events=alloc_mem(pool,sizeof(struct pollfd)*MAX_EVENT_COUNT);
  int index=0;
  while(index<MAX_EVENT_COUNT){
    events[index].fd=0;
    index++;
  };
  ec->events=events;
  ec->data=alloc_mem(poll,sizeof(void*)*MAX_EVENT_COUNT);
}

static void poll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  connection_t *conn=(connection_t *)data;
  int index=find_fd_slot(ec->events,fd);
  event_t *events=ec->events;
  if(index!=-1){
    events[index].fd=fd;
    switch(event){
    	case READ:
    		events[index].events|=POLLIN;
    		break;
    	case WRITE:
    		events[index].events|=POLLOUT;
    }
  	*(ec->data+index)=data;
  }
  conn->events|=event;
}

static int find_fd_slot(event_t *events,int fd){
  int index=0;
  int empty_index=-1;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd==fd){
      return index;
    }else if(events[index].fd==0&&empty_index==-1){
      empty_index=index;
    }
    index++;
  }
  return empty_index;
}

static void poll_del_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  connection_t *conn=(connection_t *)data;
  int index=find_fd_slot(ec->events,fd);
  event_t *events=ec->events;
  if(index!=-1){
    switch(event){
      case READ:
        events[index].events&=~POLLIN;
        break;
      case WRITE:
        events[index].events&=~POLLOUT;
    }
    *(ec->data+index)=data;
  }
  conn->events&=~event;
}

static void poll_process_event(event_context_t *ec){
  int index=0;
  event_t *events=ec->events;
  int max=0;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd!=0){
      int fd=events[index].fd;
      if(fd>max){
      	max=fd;
      }
    }
    index++;
  }
  int ret = poll(ec->events,max,timeout);
  if(ret==-1){
    my_log(ERROR,"poll event error\n");
    return;
  }else if(ret==0){
    return;
  }
  event_t *events=(event_t *)ec->events;
  int i=0;  
  for(;i<MAX_EVENT_COUNT;i++){
    int fd=events[i].fd;
    if(fd==0){
      continue;
    }
    if(events[i].revents & POLLIN>0){
      if(fd==ec->listen_fd){
        accept_connection();
      }else if(fd==ec->worker_fd){
        handle_notify(fd,ec);
      }else{
        handle_read((connection_t *)*(ec->data+i));
      }
    }
    if(events[i].revents & POLLOUT>0){
      int result=handle_write((connection_t *)*(ec->data+i));
      if(result==OK){
        event_operation.del_event(fd,WRITE,ec);
      }
    }
  }
}

static void poll_close_event(int fd,event_context_t *ec){
  event_t *events=ec->events;
  int index=0;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd==fd){
      events[index].fd=0;
    }
    index++;
  }
}
#endif