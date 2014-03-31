#include "connection.h"
#include "network.h"
#include "dy_char.h"
#include "buffer.h"
#include "queue.h"
#include "memory.h"
#include <pthread.h>
#include <stdlib.h>


extern mem_pool_t *pool;

static void destroy_connection(connection_t *conn);

void reset_read_context(read_context_t *rc){
  reset_char(rc->key);
  reset_char(rc->command);
  reset_char(rc->num);
  rc->last_bytes=0;
  rc->read_process=READ_COMMAND;
}

connection_t* init_connection(){
  connection_t *co=(connection_t *)alloc_mem(pool,sizeof(connection_t));
  co->rbuf=alloc_buffer(READ_BUF_SIZE);
  co->wbuf=alloc_buffer(WRITE_BUF_SIZE);
  read_context_t *rc=(read_context_t *)alloc_mem(pool,sizeof(read_context_t));
  co->rc=rc;
  rc->read_process=READ_COMMAND;
  rc->command=init_char();
  rc->key=init_char();
  rc->num=init_char();
  rc->last_bytes=0;
  write_context_t *wc=(write_context_t *)alloc_mem(pool,sizeof(write_context_t));
  co->wc=wc;
  wc->w_queue=init_queue();
  wc->w_index=0;
  return co;
}
void cancel_connection(connection_t *conn){
  event_operation.del_event(conn->fd,READ,conn->ec);
  event_operation.del_event(conn->fd,WRITE,conn->ec);
  destroy_connection(conn);
}

static void destroy_connection(connection_t *conn){
  destroy_char(conn->rc->command);
  destroy_char(conn->rc->key);
  destroy_char(conn->rc->num);
  free_mem(conn->rc);
  while(1){
    item_t *i=pop(conn->wc->w_queue);
    if(i==NULL){
      break;
    }
    destroy_item(i);
  }
  destroy_queue(conn->wc->w_queue);
  free_mem(conn->wc);
  destroy_buffer(conn->rbuf);
  destroy_buffer(conn->wbuf);
  free_mem(conn);
}
