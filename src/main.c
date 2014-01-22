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
#include "hash.h"
#define SERVER_PORT 10000
#define BACKLOG 50
static void start_listen();

int server_sock_fd;

hash_t *hash;

int main (int argc, const char * argv[])
{
  start_workers();
  start_listen();
  hash=init_hash();
  event_context_t ec;
  ec.listen_fd=server_sock_fd;
  event_operation.init_event(&ec);
  event_operation.add_listen_event(&ec);
  while (1) {
    event_operation.process_listen_event(&ec);
  }
  return 0;
}

static void start_listen(){  
  struct sockaddr_in server_addr;
  server_addr.sin_len = sizeof(struct sockaddr_in);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  bzero(&(server_addr.sin_zero),8);
  server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock_fd == -1) {
    printf("socket error\n");
    exit(0);
  }
  int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_result == -1) {
    perror("bind error\n");
    exit(0);
  }
  if (listen(server_sock_fd,BACKLOG) == -1) {
    perror("listen error\n");
    exit(0);
  }
}
