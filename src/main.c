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
#include <setjmp.h>
#include "thread.h"
#include "memory.h"
#include "hash.h"
#include "my_log.h"
#include "network.h"

#define SERVER_PORT 10000
#define BACKmy_log 50

static void start_listen();

extern int daemonize();

int server_sock_fd;

hash_t *hash;

mem_pool_t *pool;

jmp_buf exit_buf;


int main (int argc, const char * argv[])
{
  daemonize();
  int exit_code=0;
  if((exit_code=setjmp(exit_buf))!=0){
    exit(exit_code);
  }
  my_log_init();
  pool=init_mem_pool();
  hash=init_hash();
  start_workers();
  start_listen();
  event_context_t ec;
  ec.listen_fd=server_sock_fd;
  event_operation.init_event(&ec);
  event_operation.register_event(server_sock_fd,READ,&ec,NULL);
  while (1) {
    event_operation.process_event(&ec);
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
    my_log(ERROR,"create server socket error\n");
    longjmp(exit_buf,-1);
  }
  int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_result == -1) {
    my_log(ERROR,"bind port failed\n");
    longjmp(exit_buf,-1);
  }
  if (listen(server_sock_fd,BACKmy_log) == -1) {
    my_log(ERROR,"socket listen failed\n");
    longjmp(exit_buf,-1);
  }
}


