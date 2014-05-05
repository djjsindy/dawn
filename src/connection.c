#include <pthread.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>
#include "network.h"
#include "dy_char.h"
#include "buffer.h"
#include "queue.h"
#include "memory.h"
#include "my_log.h"
#include "config.h"
#include "connection.h"

#define READ_BUF_SIZE 1024*4
#define WRITE_BUF_SIZE 1024*4

static intptr_t read_buf_size=READ_BUF_SIZE;

static intptr_t write_buf_size=WRITE_BUF_SIZE;

static void destroy_connection(connection_t *conn);

static void init_read_context(connection_t *conn);

static void init_write_context(connection_t *conn);

static void set_read_buf_size_value(char_t *value);

static void set_write_buf_size_value(char_t *value);

extern jmp_buf exit_buf;

extern mem_pool_t *pool;

extern intptr_t parse_int(char_t *s);

/**
 *  当处理完一个command，那么需要清空read context中得数据，以便处理后面得command
 */
void reset_read_context(read_context_t *rc){
  reset_char(rc->key);
  reset_char(rc->command);
  reset_char(rc->num);
  rc->last_bytes=0;
  rc->read_process=READ_COMMAND;
}

/**
 *  初始化connection，包括读buf，写buf，读写context
 */
connection_t* init_connection(intptr_t fd){
  connection_t *co=(connection_t *)alloc_mem(pool,sizeof(connection_t));
  co->rbuf=alloc_buffer(read_buf_size);
  co->wbuf=alloc_buffer(write_buf_size);
  init_read_context(co);
  init_write_context(co);
  co->fd=fd;
  return co;
}

/**
 *  初始化read context，包括状态机初始状态，command，key，value 的长度
 */
static void init_read_context(connection_t *conn){
  read_context_t *rc=(read_context_t *)alloc_mem(pool,sizeof(read_context_t));
  conn->rc=rc;
  rc->read_process=READ_COMMAND;
  rc->command=init_char();
  rc->key=init_char();
  rc->num=init_char();
  rc->last_bytes=0;
}

/**
 *  初始化write context，主要是写队列
 */
static void init_write_context(connection_t *conn){
  write_context_t *wc=(write_context_t *)alloc_mem(pool,sizeof(write_context_t));
  conn->wc=wc;
  wc->w_data=NULL;
  wc->w_index=0;
  wc->w_queue=init_queue();
}

/**
 *  关闭connection之前，需要释放一些关于connection中得数据结构，先取消网络事件
 */
void cancel_connection(connection_t *conn){
  event_operation.del_event(conn->fd,READ,conn->ec,conn);
  event_operation.del_event(conn->fd,WRITE,conn->ec,conn);
  destroy_connection(conn);
}

/**
 *  释放connection得数据结构内存空间。
 */
static void destroy_connection(connection_t *conn){
  destroy_char(conn->rc->command);
  destroy_char(conn->rc->key);
  destroy_char(conn->rc->num);
  free_mem(conn->rc);
  destroy_queue(conn->wc->w_queue);
  free_mem(conn->wc);
  destroy_buffer(conn->rbuf);
  destroy_buffer(conn->wbuf);
  free_mem(conn);
}

static command_t connection_command[]={
  {"read_buf_size",set_read_buf_size_value},
  {"write_buf_size",set_write_buf_size_value},
  NULL
};

config_module_t connection_conf_module={
  "connection",
  connection_command
};

static void set_read_buf_size_value(char_t *value){
  read_buf_size=parse_int(value);
}

static void set_write_buf_size_value(char_t *value){
  write_buf_size=parse_int(value);
}
