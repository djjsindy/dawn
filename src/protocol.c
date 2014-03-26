#include "buffer.h"
#include "dy_char.h"
#include "hash.h"
#include "queue.h"
#include "item.h"
#include "network.h"
#include "memory.h"
#include "connection.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int process_set(connection_t *conn);

static int process_get(connection_t *conn);

static void write_set_response(connection_t *conn);

static char* fill_get_response_header(char *c,int bytes);

static char* fill_get_response_footer();

extern hash_t *hash;

extern event_operation_t event_operation;

extern mem_pool_t *pool;

static const char space=' ';

static const char special_r='\r';

static const char special_n='\n';

static const char special_end='\0';

static const char *set_response="STORED\r\n";

static const char *first_get_response="VALUE ";

static const char *data_end="END\r\n";

static const char *command_end="\r\n\0";

static const char *middle=" 0 ";

int parse_command(connection_t *conn){
  read_context_t *rc=conn->rc;
  buffer_t *buf=conn->rbuf;
  while(buf->current<buf->limit){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    switch(rc->read_process){
      case READ_COMMAND:
        if(c==space){
          add_terminal(rc->command);
          rc->read_process=READ_KEY;
        }else{
          add_char(rc->command,c);     
        }
        break;
      case READ_KEY:
        if(c==space){
          add_terminal(rc->key);
          rc->read_process=READ_FLAG;
        }else if(c==special_r){
          rc->read_process=READ_COMMAND_LAST;
        }else{
          add_char(rc->key,c);
        }
        break;
      case READ_FLAG:
        if(c==space){
          rc->read_process=READ_EXPIRE_TIME;
        }
        break;
      case READ_EXPIRE_TIME:
        if(c==space){
          rc->read_process=READ_BYTE_NUM;
        }
        break;
      case READ_BYTE_NUM:
        if(c==special_r){
          add_terminal(rc->num);
          rc->read_process=READ_COMMAND_LAST;
          rc->last_bytes=atoi(rc->num->data);
        }else{
          add_char(rc->num,c);
        }
        break;
      case READ_COMMAND_LAST:
        if(c==special_n){
          rc->read_process=READ_COMMAND_END;
          return COMMAND_OK;
        }
        break;
      case READ_COMMAND_END:
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
    //todo
    return OK;
  }
}

static int process_get(connection_t *conn){
  read_context_t *rc=conn->rc;
  queue_t *q=(queue_t *)get(rc->key->data,hash);
  int first=1;
  while(1){
    item_t *i=(item_t *)pop(q);
    if(i==NULL){
      break;
    }
    if(first){    
      push(conn->wc->w_queue,fill_get_response_header(rc->key->data,i->size));
      first=0;
    }
    push(conn->wc->w_queue,i->data);
    if(i->end==1){
      break;
    }
  }
  push(conn->wc->w_queue,fill_get_response_footer());
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
  return OK;
}

static int process_set(connection_t *conn){
  buffer_t *b=conn->rbuf;
  int count=b->limit-b->current;
  if(count==0){
    return AGAIN;
  }
  read_context_t *rc=conn->rc;
  char *key=rc->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q==NULL){
    q=init_queue();
    char *q_name=(char *)alloc_mem(pool,strlen(key));
    strcpy(q_name,key);
    put(q_name,q,hash);
  }  
  int fill=0;
  int result;
  item_t *i=init_item();
  i->size=rc->last_bytes;
  if(count-2>=rc->last_bytes){
    fill=rc->last_bytes+2;
    i->end=1;
    result=OK;
    write_set_response(conn);
  }else{
    fill=count;
    result=AGAIN;
    rc->last_bytes-=fill;
  }
  char *c=(char *)alloc_mem(pool,fill+1);
  memset(c,0,fill);
  memcpy(c,b->data+b->current,fill);
  *(c+fill)=special_end;
  i->data=c;
  push(q,i);
  b->current+=fill;
  return result;
}

static void write_set_response(connection_t *conn){
  char *c=(char *)alloc_mem(pool,strlen(set_response));
  strcpy(c,set_response);
  push(conn->wc->w_queue,c);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}

static char* fill_get_response_header(char *key,int bytes){
  char *s=(char *)alloc_mem(pool,20);
  sprintf(s,"%d",bytes);
  int first_length=strlen(first_get_response);
  int key_length=strlen(key);
  int middle_length=strlen(middle);
  int s_length=strlen(s);
  int command_end_length=strlen(command_end);
  int length=first_length+key_length+middle_length+s_length+command_end_length+1;
  char *c=(char *)alloc_mem(pool,length);
  memset(c,0,length);
  int index=0;
  memcpy(c,first_get_response,first_length);
  index+=first_length;
  memcpy(c+index,key,key_length);
  index+=key_length;
  memcpy(c+index,middle,middle_length);
  index+=middle_length;
  memcpy(c+index,s,s_length);
  index+=s_length;
  memcpy(c+index,command_end,command_end_length);
  index+=command_end_length;
  *(c+index)=special_end;
  free_mem(s);
  return c;
}

static char* fill_get_response_footer(){
  char *c=(char *)alloc_mem(pool,strlen(data_end)+1);
  strcpy(c,data_end);
  return c;
}
