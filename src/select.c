#ifdef HAVE_SELECT
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include "memory.h"
#include "list.h"
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <errno.h> 

struct event_s{
  void *data;
  int fd;
  enum EVENT event;
};

struct timeval tv={5,0};

typedef struct event_s event_t;

static void select_init_event(event_context_t *ec);

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void select_del_event(int fd,enum EVENT event,event_context_t *ec);

static void select_process_event(event_context_t *ec);

static void select_close_event(int fd,event_context_t *ec);

static int find_empty_slot(event_t *events);

static int prepare_fd_set(event_context_t *ec,fd_set *read_set,fd_set *write_set);

extern int worker_index;

extern int server_sock_fd;

extern thread_t threads[WORKER_NUM];

extern int set_noblocking(int fd);

extern jmp_buf exit_buf;

extern mem_pool_t *pool;

event_operation_t event_operation={
  select_init_event,
  select_register_event,
  select_del_event,
  select_process_event,
  select_close_event
};

static void select_init_event(event_context_t *ec){ 
  event_t *events=alloc_mem(pool,sizeof(event_t)*MAX_EVENT_COUNT);
  int index=0;
  while(index<MAX_EVENT_COUNT){
    events[index].fd=0;
    index++;
  }
  ec->events=events;
}

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  int index=find_empty_slot(ec->events);
  event_t *events=ec->events;
  if(index!=-1){
    events[index].data=data;
    events[index].fd=fd;
    events[index].event=event;
  }
}

static int find_empty_slot(event_t *events){
  int index=0;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd==0){
      return index;
    }
    index++;
  }
  return -1;
}

static void select_del_event(int fd,enum EVENT event,event_context_t *ec){
  int index=0;
  event_t *events=ec->events;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd==fd&&events[index].event==event){
      events[index].fd=0;
    }
    index++;
  }
}

static void select_process_event(event_context_t *ec){
  fd_set read_set;
  fd_set write_set;
  int max=prepare_fd_set(ec,&read_set,&write_set);
  int ret = select(max+1, &read_set, &write_set, NULL, &tv);
  if(ret==-1){
    my_log(ERROR,"select event error\n");
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
    if(FD_ISSET(fd,&read_set)){
      if(fd==ec->listen_fd){
        accept_connection();
      }else if(fd==ec->worker_fd){
        handle_notify(fd,ec);
      }else{
        handle_read(events[i].data);
      }
    }
    if(FD_ISSET(fd,&write_set)){
      int result=handle_write(events[i].data);
      if(result==OK){
        event_operation.del_event(fd,WRITE,ec);
      }
    }
  }
}

static int prepare_fd_set(event_context_t *ec,fd_set *read_set,fd_set *write_set){
  int max=0;
  FD_ZERO(read_set);
  FD_ZERO(write_set);
  int index=0;
  event_t *events=(event_t *)ec->events;
  for(;index<MAX_EVENT_COUNT;index++){    
    if(events[index].fd==0){
      continue;
    }
    if(events[index].event==READ){
      FD_SET(events[index].fd,read_set);
    }
    if(events[index].event==WRITE){
      FD_SET(events[index].fd,write_set);
    }
    if(events[index].fd>max){
      max=events[index].fd;
    }
  }
  return max;
}

static void select_close_event(int fd,event_context_t *ec){
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