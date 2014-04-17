#ifdef HAVE_EPOLL
#include "network.h"
#include "thread.h"
#include "my_log.h"
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#define MAX_FD_COUNT 1024

static void epoll_init_event(event_context_t *ec);

static void epoll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void epoll_del_event(int fd,enum EVENT event,event_context_t *ec,void *data);

static void epoll_process_event(event_context_t *ec);

static void epoll_close_event(int fd,event_context_t *ec);

extern int server_sock_fd;

extern jmp_buf exit_buf;

extern mem_pool_t *pool;

static const int timeout=1;

event_operation_t event_operation={
	epoll_init_event,
	epoll_register_event,
	epoll_del_event,
	epoll_process_event,
	epoll_close_event
};

static void epoll_init_event(event_context_t *ec){ 
	int fd = epoll_create(MAX_FD_COUNT);
	if (fd == -1) {
		my_log(ERROR,"create epoll fd\n");
		longjmp(exit_buf,-1);
	}
	ec->fd=fd;
	ec->events=alloc_mem(pool,sizeof(struct epoll_event)*MAX_EVENT_COUNT);
}

static void epoll_register_event(int fd,enum EVENT event,event_context_t *ec,void *data){
	connection_t *conn=(connection_t *)data;
	struct epoll_event ev;
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
	ev.data.fd=fd;
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
	int nfds = epoll_wait(ec->fd,ec->events,MAX_EVENT_COUNT,timeout);
	if(ret==-1){
		my_log(ERROR,"epoll event error\n");
		return;
	}else if(ret==0){
		return;
	}
	int i=0;
	struct epoll_event *events=(struct epoll_event *)ec->events;
	for(;i<nfds;i++){
		int sockfd= events[i].data.fd; 
		if(sockfd==ec->worker_fd){
			handle_notify(sockfd,ec);
		}else if(sockfd==ec->listen_fd){
			accept_connection();
		}else{
			connection_t *conn=(connection_t *)events[i].data.ptr;
			if(events[i].events|EPOLLIN>0){
				handle_read(conn);
			}else if(events[i].events|EPOLLOUT>0){
				int result=handle_write(conn);
				if(result==OK){
					event_operation.del_event(sockfd,WRITE,ec);
				}
			}
		}
	}
}

static void epoll_close_event(int fd,event_context_t *ec){
  	struct epoll_event ev;
	epoll_ctl(ec->fd,EPOLL_CTL_DEL,fd,&ev);
}
#endif