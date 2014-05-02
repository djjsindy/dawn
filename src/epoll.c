#ifdef HAVE_EPOLL
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include "config.h"
#include "memory.h"

static void epoll_init_event(event_context_t *ec);

static void epoll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void epoll_del_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void epoll_process_event(event_context_t *ec);

static void epoll_close_event(int fd,event_context_t *ec);

static void set_timeout_value(char_t *value);

static void set_max_event_count_value(char_t *value);

static int timeout=TIMEOUT;

static int max_event_count=MAX_EVENT_COUNT;

extern int server_sock_fd;

extern mem_pool_t *pool;

event_operation_t event_operation={
	epoll_init_event,
	epoll_register_event,
	epoll_del_event,
	epoll_process_event,
	epoll_close_event
};

static void epoll_init_event(event_context_t *ec){ 
	int fd = epoll_create(max_event_count);
	if (fd == -1) {
		my_log(ERROR,"create epoll fd\n");
	}
	ec->fd=fd;
	ec->events=alloc_mem(pool,sizeof(struct epoll_event)*max_event_count);
}

static void epoll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
	connection_t *conn=(connection_t *)data;
	struct epoll_event ev;
        int op;
	if(conn->events==0){
		op=EPOLL_CTL_ADD;
		switch(event){
		case READ:
			ev.events=EPOLLIN;
			break;
		case WRITE:
			ev.events=EPOLLOUT;
			break;
		}
	}else{
		op=EPOLL_CTL_MOD;
		ev.events=EPOLLIN|EPOLLOUT;
	}
	ev.data.ptr=data;
	epoll_ctl(ec->fd,op,fd,&ev);
	conn->events|=event;
}

static void epoll_del_event(int fd,enum EVENT event,event_context_t *ec,void *data){
	connection_t *conn=(connection_t *)data;
	struct epoll_event ev;
	if(conn->events|READ>0){
		ev.events|=EPOLLIN;
	}else if(conn->events|WRITE>0){
		ev.events|=EPOLLOUT;
	}
	switch(event){
		case READ:
			ev.events&=~EPOLLIN;
			break;
		case WRITE:
			ev.events&=~EPOLLOUT;
			break;
	}
	ev.data.fd=fd;
	ev.data.ptr=data;
	epoll_ctl(ec->fd,EPOLL_CTL_MOD,fd,&ev);
	conn->events&=~event;
}


static void epoll_process_event(event_context_t *ec){
	int nfds = epoll_wait(ec->fd,ec->events,max_event_count,timeout);
	if(nfds==-1){
		my_log(ERROR,"epoll event error\n");
		return;
	}else if(nfds==0){
		return;
	}
	int i=0;
	struct epoll_event *events=(struct epoll_event *)ec->events;
	for(;i<nfds;i++){
		connection_t *conn=events[i].data.ptr;
		int sockfd= conn->fd; 
		if(sockfd==ec->worker_fd){
			handle_notify(sockfd,ec);
		}else if(sockfd==ec->listen_fd){
			accept_connection();
		}else{
			if(events[i].events|EPOLLIN>0){
				handle_read(conn);
			}else if(events[i].events|EPOLLOUT>0){
				int result=handle_write(conn);
				if(result==OK){
					event_operation.del_event(sockfd,WRITE,ec,conn);
				}
			}
		}
	}
}

static void epoll_close_event(int fd,event_context_t *ec){
  	struct epoll_event ev;
	epoll_ctl(ec->fd,EPOLL_CTL_DEL,fd,&ev);
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
