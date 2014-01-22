#include "buffer.h"
#include "dy_char.h"
#include "hash.h"
#include "queue.h"
#include "thread.h"
#include "item.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern hash_t* hash;

static int process_set(connection_t *conn);

static int process_get(connection_t *conn);

int parse_command(connection_t *conn){
  buffer_t *buf=conn->rbuf;
  while(buf->current<=buf->limit){
    char c=*(buf->data+buf->current);
    switch(conn->read_process){
      case READ_COMMAND:
        if(c==' '){
          add_terminal(conn->command);
          conn->read_process=READ_KEY;
        }else{
           add_char(conn->command,c);     
        }
        break;
      case READ_KEY:
        if(c==' '){
          add_terminal(conn->key);
          conn->read_process=READ_FLAG;
        }else{
          add_char(conn->key,c);
        }
        break;
      case READ_FLAG:
        if(c==' '){
          conn->read_process=READ_EXPIRE_TIME;
        }
        break;
      case READ_EXPIRE_TIME:
        if(c==' '){
          conn->read_process=READ_BYTE_NUM;
        }
        break;
      case READ_BYTE_NUM:
        if(c=='\r'){
          conn->read_process=READ_COMMAND_LAST;
          sprintf(conn->num,"%d",conn->last_bytes);
        }else{
          add_char(conn->num,c);
        }
        break;
      case READ_COMMAND_LAST:
        if(c=='\n'){
          conn->read_process=READ_COMMAND_END;
          return COMMAND_OK;
        }
        break;
      case READ_TERMINAL:
        if(c=='\r')
          conn->read_process=READ_DATA_END;
        break;
      case READ_DATA_END:
        if(c=='\n')
          return DATA_OK;
    }
    buf->current+=1;
  }
  return AGAIN;
}

int process_command(connection_t *conn){
  if(strcmp(conn->command->data,"set")==0)
      return process_set(conn);
  else if(strcmp(conn->command->data,"get")==0)
      return process_get(conn);
}
static int process_get(connection_t *conn){
  return 0;
}

static int process_set(connection_t *conn){
  buffer_t *b=conn->rbuf;
  char* key=conn->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q==NULL){
    q=init_queue();
    put(key,q,hash);
  } 
  int count=b->limit-b->current; 
  int fill;
  int result;
  item_t *i=init_item();
  if(count>conn->last_bytes){
    fill=conn->last_bytes;
    i->end=1;
    conn->read_process=READ_TERMINAL;
    result=OK;
  }else{
    fill=b->limit-b->current;
    result=AGAIN;
  }
  char *c=(char *)malloc(fill);
  memcpy(c,b->data,fill);
  push(q,c);
  return result;
}
