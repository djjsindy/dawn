#ifdef HAVE_SELECT
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include "memory.h"
#include "list.h"

struct event_s{
  void *data;
  int fd;
  enum EVENT event;
};

typedef struct event_s event_t;

static void select_init_event(event_context_t *ec);

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void select_del_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void select_process_event(event_context_t *ec);

static void select_close_event(int fd,event_context_t *ec);

static int find_fd_slot(event_t *events,int fd);

static int prepare_fd_set(event_context_t *ec,fd_set *read_set,fd_set *write_set);

static void set_timeout_value(char_t *value);

static void set_max_event_count_value(char_t *value);

static int timeout=TIMEOUT;

static int max_event_count=MAX_EVENT_COUNT;

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
  event_t *events=alloc_mem(pool,sizeof(event_t)*max_event_count);
  int index=0;
  while(index<max_event_count){
    events[index].fd=0;
    index++;
  }
  ec->events=events;
}

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  connection_t *conn=(connection_t *)data;
  int index=find_fd_slot(ec->events,fd);
  event_t *events=ec->events;
  if(index!=-1){
    events[index].data=data;
    events[index].fd=fd;
    events[index].event|=event;
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

static void select_del_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  connection_t *conn=(connection_t *)data;
  int index=0;
  event_t *events=ec->events;
  while(index<MAX_EVENT_COUNT){
    if(events[index].fd==fd){
      events[index].event&=~event;
      break;
    }
    index++;
  }
  conn->events&=~event;
}

static void select_process_event(event_context_t *ec){
  struct timeval tv={timeout,0};
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
  for(;i<max_event_count;i++){
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
  for(;index<max_event_count;index++){    
    if(events[index].fd==0){
      continue;
    }
    if(events[index].event&READ!=0){
      FD_SET(events[index].fd,read_set);
    }
    if(events[index].event&WRITE!=0){
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
  while(index<max_event_count){
    if(events[index].fd==fd){
      events[index].fd=0;
    }
    index++;
  }
}

static command_t events_command[]={
  {"timeout",set_timeout_value},
  {"max_event_count",set_max_event_count_value},
  NULL
};

config_module_t events_conf_module={
  "events",
  events_command
};

static void set_timeout_value(char_t *value){
  timeout=atoi(value->data);
}

static void set_max_event_count_value(char_t *value){
  max_event_count=atoi(value->data);
}
#endif