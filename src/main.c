#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "thread.h"
#include "memory.h"
#include "hash.h"
#include "my_log.h"
#include "network.h"
#include "connection.h"
#include "config.h"
#include "statistics.h"

#define DEFAULT_PORT 10000

#define DEFAULT_BACKLOG 50

static void start_listen();

static void wait_connection();

static void set_port_value(char_t *value);

static void set_back_log_value(char_t *value);

extern int daemonize();

extern void init_sync();

extern void init_conf();

int server_sock_fd;

hash_t *hash;

mem_pool_t *pool;

static int port=DEFAULT_PORT;

static int back_log=DEFAULT_BACKLOG;

int main (int argc, const char * argv[])
{ 
  //后台进程
  // daemonize();

  //初始化log系统
  my_log_init();

  //初始化统计
  init_stat();

  //初始全局内存池
  pool=init_mem_pool();

  //初始化队列统计hash
  init_queue_stat();

  //从配置文件中按照module加载配置
  init_conf();

  //初始化存储队列的hash结构
  hash=init_hash();

  //根据持久化文件，初始化数据，开启同步数据线程
  //init_sync();

  //启动worker线程
  start_workers();

  //建立端口连接
  start_listen();

  //主线程监听端口，等待连接
  wait_connection();
  return 0;
}

static void wait_connection(){
  event_context_t ec;
  ec.listen_fd=server_sock_fd;
  connection_t *conn=init_connection();
  event_operation.init_event(&ec);
  event_operation.register_event(server_sock_fd,READ,&ec,conn);
  while (1) {
    event_operation.process_event(&ec);
  }
}

static void start_listen(){  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  bzero(&(server_addr.sin_zero),8);
  server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock_fd == -1) {
    my_log(ERROR,"create server socket error\n");
  }
  int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (bind_result == -1) {
    my_log(ERROR,"bind port failed\n");
  }
  if (listen(server_sock_fd,back_log) == -1) {
    my_log(ERROR,"socket listen failed\n");
  }
}

static command_t listen_command[]={
  {"port",set_port_value},
  {"back_log",set_back_log_value},
  NULL
};

config_module_t listen_conf_module={
  "listen",
  listen_command
};

static void set_port_value(char_t *value){
  port=atoi(value->data);
}

static void set_back_log_value(char_t *value){
  back_log=atoi(value->data);
}
