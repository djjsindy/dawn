#include "buffer.h"
#include "dy_char.h"
#include "hash.h"
#include "queue.h"
#include "thread.h"
#include "item.h"
#include "network.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *response="STORED\r\n";

static int process_set(connection_t *conn);

static int process_get(connection_t *conn);

static void write_set_response(connection_t *conn);

extern hash_t *hash;

extern event_operation_t event_operation;

int parse_command(connection_t *conn){
  read_context_t *rc=conn->rc;
  if(rc->read_process==END){
    return OK;
  }
  buffer_t *buf=conn->rbuf;
  while(buf->current<buf->limit){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    switch(rc->read_process){
      case READ_COMMAND:
        if(c==' '){
          add_terminal(rc->command);
          rc->read_process=READ_KEY;
        }else{
          add_char(rc->command,c);     
        }
        break;
      case READ_KEY:
        if(c==' '){
          add_terminal(rc->key);
          rc->read_process=READ_FLAG;
        }else if(c=='\r'){
          rc->read_process=READ_COMMAND_LAST;
        }else{
          add_char(rc->key,c);
        }
        break;
      case READ_FLAG:
        if(c==' '){
          rc->read_process=READ_EXPIRE_TIME;
        }
        break;
      case READ_EXPIRE_TIME:
        if(c==' '){
          rc->read_process=READ_BYTE_NUM;
        }
        break;
      case READ_BYTE_NUM:
        if(c=='\r'){
          add_terminal(rc->num);
          rc->read_process=READ_COMMAND_LAST;
          rc->last_bytes=atoi(rc->num->data);
        }else{
          add_char(rc->num,c);
        }
        break;
      case READ_COMMAND_LAST:
        if(c=='\n'){
          rc->read_process=READ_COMMAND_END;
          return COMMAND_OK;
        }
        break;
      case READ_COMMAND_END:
      case END:
        break;
      case READ_TERMINAL:
        if(c=='\r')
          rc->read_process=READ_DATA_END;
        break;
      case READ_DATA_END:
        if(c=='\n'){
          return OK;
        }
        break;
    }
  }
  return AGAIN;
}

int process_command(connection_t *conn){
  if(strcmp(conn->rc->command->data,"set")==0)
    return process_set(conn);
  else if(strcmp(conn->rc->command->data,"get")==0)
    return process_get(conn);
  else{
    printf("unsupport operation\n");
    exit(0);
  }
}
static int process_get(connection_t *conn){
  read_context_t *rc=conn->rc;
  queue_t *q=(queue_t *)get(rc->key->data,hash);
  while(1){
    item_t *i=(item_t *)pop(q);
    if(i==NULL)
      break;
    push(conn->wc->w_queue,i->data);
    if(i->end==1)
      break;
  }
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
  rc->read_process=END;
  return CONTINUE;
}

static int process_set(connection_t *conn){
  buffer_t *b=conn->rbuf;
  int count=b->limit-b->current;
  if(count==0){
    return AGAIN;
  }
  read_context_t *rc=conn->rc;
  char* key=rc->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q==NULL){
    q=init_queue();
    char *q_name=(char *)malloc(strlen(key));
    strcpy(q_name,key);
    put(q_name,q,hash);
  }  
  int fill;
  int result;
  item_t *i=init_item();
  if(count-2>=rc->last_bytes){
    fill=rc->last_bytes;
    i->end=1;
    rc->read_process=READ_TERMINAL;
    result=CONTINUE;
    write_set_response(conn);
  }else{
    fill=b->limit-b->current;
    result=AGAIN;
  }
  char *c=(char *)malloc(fill);
  memcpy(c,b->data+b->current,fill);
  i->data=c;
  push(q,i);
  b->current+=fill;
  return result;
}

static void write_set_response(connection_t *conn){
  char *c=(char *)malloc(8);
  memcpy(c,response,8);
  push(conn->wc->w_queue,c);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}
