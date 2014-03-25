#include "connection.h"
#include "network.h"
#include "dy_char.h"
#include "buffer.h"
#include "item.h"
#include "queue.h"
#include <stdlib.h>
#define KEY_SIZE 16
#define COMMAND_SIZE 16
#define NUM_SIZE 16

static void destroy_connection(connection_t *conn);

void reset_read_context(read_context_t *rc){
  reset_char(rc->key);
  reset_char(rc->command);
  reset_char(rc->num);
  rc->last_bytes=0;
  rc->read_process=READ_COMMAND;
}

connection_t* init_connection(){
  connection_t *co=(connection_t *)malloc(sizeof(connection_t));
  co->rbuf=alloc_buffer(READ_BUF_SIZE);
  co->wbuf=alloc_buffer(WRITE_BUF_SIZE);
  read_context_t *rc=(read_context_t *)malloc(sizeof(read_context_t));
  co->rc=rc;
  rc->read_process=READ_COMMAND;
  rc->command=init_char(COMMAND_SIZE);
  rc->key=init_char(KEY_SIZE);
  rc->num=init_char(NUM_SIZE);
  rc->last_bytes=0;
  write_context_t *wc=(write_context_t *)malloc(sizeof(write_context_t));
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
  free(conn->rc);
  while(1){
    item_t *i=pop(conn->wc->w_queue);
    if(i==NULL){
      break;
    }
    destroy_item(i);
  }
  destroy_queue(conn->wc->w_queue);
  free(conn->wc);
  destroy_buffer(conn->rbuf);
  destroy_buffer(conn->wbuf);
  free(conn);
}
