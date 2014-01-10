#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "thread.h"
#define SERVER_PORT 11332
#define BUFFER_SIZE 1024
#define MAX_EVENT_COUNT 1024
#define BACKLOG 50
struct kevent events[MAX_EVENT_COUNT];
extern thread_t *threads;
int worker_index=0;
char notify_buf[32];

int main (int argc, const char * argv[])
{
  start_workers();
  char input_msg[BUFFER_SIZE];
  char recv_msg[BUFFER_SIZE];
  struct sockaddr_in server_addr;
  server_addr.sin_len = sizeof(struct sockaddr_in);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  bzero(&(server_addr.sin_zero),8);
  //创建socket
  int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock_fd == -1) {
    perror("socket error");
    return 1;
  }
  //绑定socket
  int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_result == -1) {
    perror("bind error");
    return 1;
  }
  //listen
  if (listen(server_sock_fd,BACKLOG) == -1) {
    perror("listen error");
    return 1;
  }
  //kqueue
  int kq = kqueue();
  if (kq == -1) {
    perror("创建kqueue出错!\n");
    exit(1);
  }
  struct kevent event_change;
  EV_SET(&event_change, server_sock_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  kevent(kq, &event_change, 1, NULL, 0, NULL);
  while (1) {
    int ret = kevent(kq, NULL, 0, events,MAX_EVENT_COUNT , NULL);
    if (ret < 0) {
      printf("kevent 出错!\n");
      continue;
    }else if(ret == 0){
      continue;
    }
    for (int i = 0; i < ret; i++) {
      struct kevent current_event = events[i];
      if(current_event.ident == server_sock_fd){
        //有新的连接请求
        struct sockaddr_in client_address;
        socklen_t address_len;
        int client_socket_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);
        *notify_buf='c';
        sprintf(notify_buf+1,"%d",client_socket_fd);
        thread_t *t=&threads[worker_index];
        pipe_channel_t* p=&(t->pipe_channel);
        int fd=p->masterfd;
        write(fd,notify_buf,strlen(notify_buf));
        worker_index++;
        if(worker_index==WORKER_NUM){
           worker_index=0;
        }
      }     
    }
  }
  return 0;
}
