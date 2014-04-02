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

int max_fd;

struct event_s{
  void *data;
  int fd;
  int event_type;
};

struct timeval tv={1,0};

typedef struct event_s event_t;

static void select_init_event(event_context_t *ec);

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void select_del_event(int fd,enum EVENT event,event_context_t *ec);

static void select_process_event(event_context_t *ec);

static int insert_event(int fd,int index,event_t *events);

static int find_event(int fd,int start,int end,event_t *events);

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
  select_process_event
};

static void select_init_event(event_context_t *ec){ 
  event_t events[MAX_EVENT_COUNT];
  ec->events=events;
  ec->fd_num=0;
}

static void select_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
  event_t *events=ec->events;
  int index=find_event(fd,0,ec->fd_num,events);
  if(index==-1){
    index=insert_event(fd,ec->fd_num,events);
  }
  events[index].data=data;
  events[index].event_type=events[index].event_type|event;
  events[index].fd=fd;
  ec->fd_num+=1;
}

static int insert_event(int fd,int index,event_t *events){
  while(index>0){
    if(events[index-1].fd>fd){
      event_t t=events[index];
      events[index]=events[index-1];
      events[index-1]=t;
      index--;
    }else{
      break;
    }
  }
  return index;
}

static int find_event(int fd,int start,int end,event_t *events){
  if(start>end){
    return -1;
  }
  int mid=start+(end-start)/2;
  if(fd==events[mid].fd){
    return mid;
  }else if(events[mid].fd>fd){
    return find_event(fd,mid+1,end,events);
  }else 
    return find_event(fd,start,mid-1,events);
}

static void select_del_event(int fd,enum EVENT event,event_context_t *ec){
  event_t *events=ec->events;
  int index=find_event(fd,0,ec->fd_num,events);
  if(index!=-1){
    events[index].event_type&=~event;
  }
}

static void select_process_event(event_context_t *ec){
  int max=0;
  fd_set read_set;
  fd_set write_set;
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  int index=0;
  for(;index<ec->fd_num;index++){
    event_t *event=((event_t *)ec->events)+index;
    if((event->event_type&READ)!=0){
      FD_SET(event->fd,&read_set);
    } 
    if((event->event_type&WRITE)!=0){
      FD_SET(event->fd,&write_set);
    }
    if(event->fd>max){
      max=event->fd;
    }
  }
  int ret = select(max+1, &read_set, &write_set, NULL, &tv);
  if(ret==-1){
    my_log(ERROR,"kqueue event error\n");
    return;
  }else if(ret==0){
    return;
  }
  int i=0;
  for(;i<ec->fd_num;i++){
    event_t *events=(event_t *)ec->events;
    int fd=events[i].fd;
    if(FD_ISSET(fd,&read_set)){
      if(fd==ec->worker_fd){
        handle_notify(fd,ec);
      }else if(fd==ec->listen_fd){
        accept_connection();
      }else{
        handle_read(events[i].data);
      }
    }else if(FD_ISSET(fd,&write_set)){
      int result=handle_write(events[i].data);
      if(result==OK){
        event_operation.del_event(fd,WRITE,ec);
      }
    }
  }
}
#endif