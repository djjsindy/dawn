#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"
#include "dy_char.h"
#include "hash.h"
#include "queue.h"
#include "item.h"
#include "network.h"
#include "memory.h"
#include "connection.h"
#include "thread.h"
#include "statistics.h"

#define STAT_LINE_BUFFER_SIZE 100

static int process_set(connection_t *conn);

static int process_get(connection_t *conn);

static int process_delete(connection_t *conn);

static void process_client_error(connection_t *conn,const char *message);

static void process_version(connection_t *conn);

static void process_stats(connection_t *conn);

static int parse_set_header(connection_t *conn);

static int parse_get_header(connection_t *conn);

static int parse_delete_header(connection_t *conn);

static int process_set_body(connection_t *conn);

static void write_set_response(connection_t *conn);

static item_t* fill_get_response_header(char *c,int bytes);

static item_t* fill_get_response_footer();

static void write_delete_response(connection_t *conn);

static void write_null(connection_t *conn,char *key);

static item_t *get_buddy_stat(unsigned long *data,const char *type);

static item_t *get_direct_stat();

static void get_queue_stat(connection_t *conn);

static const char space=' ';

static const char special_r='\r';

static const char special_n='\n';

static const char special_end='\0';

static const char *set_response="STORED\r\n";

static const char *first_get_response="VALUE ";

static const char *data_end="END\r\n";

static const char *command_end="\r\n";

static const char *middle=" 0 ";

static const char *client_error="CLIENT_ERROR %s\r\n";

static const char *unsupport_command="unsupport_command";

static const char *version="VERSION 1.0\r\n";

static const char *empty="";

static const char *deleted="DELETED\r\n";

static const char *small_buddy="small bin";

static const char *big_buddy="big bin";

extern hash_t *hash;

extern event_operation_t event_operation;

extern mem_pool_t *pool;

extern stat_t *stat; 

extern void add_set_sync_data(char *key,item_t *i);

extern void add_get_sync_data(char *key,int size);

int parse_command(connection_t *conn){
  int result=AGAIN;
  read_context_t *rc=conn->rc;
  buffer_t *buf=conn->rbuf;
  while(has_remaining(buf)){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    if(c==space){
      add_terminal(rc->command);
      result=OK;
      break;
    }else if(c==special_r){
      add_terminal(rc->command);
    }else if(c==special_n){
      result=OK;
      break;
    }else{
      add_char(rc->command,c);     
    }    
  }
  if(strcmp(rc->command->data,"delete")==0||strcmp(rc->command->data,"get")==0||strcmp(rc->command->data,"set")==0){
    rc->read_process=READ_KEY;
  }
  return result;
}

int process_command(connection_t *conn){
  int result=OK;
  char *data=conn->rc->command->data;
  if(strcmp(data,"get")==0){
    result=process_get(conn);
  }else if(strcmp(data,"set")==0){
    result=process_set(conn);
  }else if(strcmp(data,"version")==0){
    process_version(conn);
  }else if(strcmp(data,"delete")==0){
    result=process_delete(conn);
  }else if(strcmp(data,"quit")==0){
    close_connection(conn);
  }else if(strcmp(data,"stats")==0){
    process_stats(conn);
  }else{
   process_client_error(conn,unsupport_command);
 }
 return result;
}

static void process_version(connection_t *conn){
  char *c=(char *)alloc_mem(pool,strlen(version)+1);
  item_t *i=init_item();
  i->data=c;
  i->c_size=strlen(version);
  i->end=1;
  strcpy(c,version);
  push(conn->wc->w_queue,i);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}

static void process_client_error(connection_t *conn,const char *message){
  int length=strlen(client_error)-2+strlen(message)+1;
  char *c=(char *)alloc_mem(pool,length);
  sprintf(c,client_error,message);
  push(conn->wc->w_queue,c);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}

static int process_get(connection_t *conn){
  read_context_t *rc=conn->rc;
  if(rc->read_process!=READ_COMMAND_END){
    int result=parse_get_header(conn);
      //表示set command第一行没有read全
    if(result==AGAIN){
      return AGAIN;
    }
  }
  char *key=rc->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q!=NULL){
    int first=1;
    while(1){
      item_t *i=(item_t *)pop(q);
      if(i==NULL){
        break;
      }
      if(first){    
        push(conn->wc->w_queue,fill_get_response_header(key,i->size));
        first=0;
        add_get_sync_data(key,i->size);
      }
      push(conn->wc->w_queue,i);
      if(i->end==1){
          //加入统计
        decrease_queue_stat(key,i->size);
        break;
      }
    }
  }

  push(conn->wc->w_queue,fill_get_response_footer());
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
  return OK;
}

static void write_null(connection_t *conn,char *key){
  int length=strlen(data_end);
  char *s=(char *)alloc_mem(pool,length);
  memcpy(s,data_end,length);
  item_t *i=(item_t *)alloc_mem(pool,length);
  i->data=s;
  i->c_size=length;
  push(conn->wc->w_queue,i);
}

static int parse_get_header(connection_t *conn){
  read_context_t *rc=conn->rc;
  buffer_t *buf=conn->rbuf;
  while(buf->current<buf->limit){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    switch(rc->read_process){
      case READ_KEY:
      if(c==special_r){
        add_terminal(rc->key);
        rc->read_process=READ_COMMAND_LAST;
      }else{
        add_char(rc->key,c);
      }
      break;
      case READ_COMMAND_LAST:
      if(c==special_n){
        rc->read_process=READ_COMMAND_END;
        return COMMAND_OK;
      }
      break;
      default:
      break;   
    }
  }
  return AGAIN;
}

static int process_set(connection_t *conn){

  if(conn->rc->read_process!=READ_COMMAND_END){
    int result=parse_set_header(conn);
      //表示set command第一行没有read全
    if(result==AGAIN){
      return AGAIN;
    }
  }
  return process_set_body(conn);

} 
static int process_set_body(connection_t *conn){ 
    //check一下当前buffer数据的剩余多少
  buffer_t *b=conn->rbuf;
  int count=b->limit-b->current;
  if(count==0){
    return AGAIN;
  }
    //开始存储set数据
  read_context_t *rc=conn->rc;
  char *key=rc->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q==NULL){

    //初始化队列，加入hash中
    q=init_queue();
    char *q_name=(char *)alloc_mem(pool,strlen(key));
    strcpy(q_name,key);
    put(q_name,q,hash);

    //开始统计这个队列
    start_queue_stat(q_name);
  }  
  int fill=0;
  int result=AGAIN;
  item_t *i=init_item();
  i->size=rc->last_bytes-2;
    //如果当前buffer的数据包含了所有的set数据包括最后的/r/n
  if(count>=rc->last_bytes){
    fill=rc->last_bytes;
    i->end=1;
    result=OK;
    write_set_response(conn);

    //加入队列数据统计
    increase_queue_stat(key,i->size);
  }else{
      //如果buffer不够了那么先把这部分数据copy出来，放入一个item中
    fill=count;
    rc->last_bytes-=fill;
  }
  char *c=(char *)alloc_mem(pool,fill);
  memset(c,0,fill);
  memcpy(c,b->data+b->current,fill);
  i->data=c;
  i->c_size=fill;
  push(q,i);
  add_set_sync_data(key,i);
  b->current+=fill;
  return result;
}

static int parse_set_header(connection_t *conn){
  read_context_t *rc=conn->rc;
  buffer_t *buf=conn->rbuf;
  while(has_remaining(buf)){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    switch(rc->read_process){
      case READ_KEY:
      if(c==space){
        add_terminal(rc->key);
        rc->read_process=READ_FLAG;
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
        rc->last_bytes=atoi(rc->num->data)+2;
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
      default:
      break;   
    }
  }
  return AGAIN;
}

static void write_set_response(connection_t *conn){
  int length=strlen(set_response);
  char *c=(char *)alloc_mem(pool,length);
  memcpy(c,set_response,length);
  item_t *i=init_item();
  i->data=c;
  i->c_size=length;
  push(conn->wc->w_queue,i);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}

static item_t* fill_get_response_header(char *key ,int bytes){
  char *s=(char *)alloc_mem(pool,20);
  sprintf(s,"%d",bytes);
  int first_length=strlen(first_get_response);
  int key_length=strlen(key);
  int middle_length=strlen(middle);
  int s_length=strlen(s);
  int command_end_length=strlen(command_end);
  int length=first_length+key_length+middle_length+s_length+command_end_length;
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
  
  free_mem(s);
  item_t *i=init_item();
  i->data=c;
  i->c_size=length;
  return i;
}

static item_t* fill_get_response_footer(){
  int length=strlen(data_end);
  char *c=(char *)alloc_mem(pool,length);
  memcpy(c,data_end,length);
  item_t *i=init_item();
  i->data=c;
  i->c_size=length;
  return i;
}

static int process_delete(connection_t *conn){
  read_context_t *rc=conn->rc;
  if(rc->read_process!=READ_COMMAND_END){
    int result=parse_delete_header(conn);
    if(result==AGAIN){
      return AGAIN;
    }
  }
  char *key=rc->key->data;
  queue_t *q=(queue_t *)get(key,hash);
  if(q!=NULL){
    while(1){
      item_t *i=(item_t *)pop(q);
      if(i==NULL){
        break;
      }
      add_get_sync_data(key,i->size);
      destroy_item(i);    
    }
  } 
  write_delete_response(conn);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
  return OK;
}

static void write_delete_response(connection_t *conn){
  item_t *i=init_item();
  int length=strlen(deleted);
  char *del=(char *)alloc_mem(pool,length);
  memcpy(del,deleted,length);
  i->data=del;
  i->c_size=length;
  push(conn->wc->w_queue,i);
}

static int parse_delete_header(connection_t *conn){
  read_context_t *rc=conn->rc;
  buffer_t *buf=conn->rbuf;
  while(has_remaining(buf)){
    char c=*(buf->data+buf->current);
    buf->current+=1;
    switch(rc->read_process){
      case READ_KEY:
      if(c==special_r){
        add_terminal(rc->key);
        rc->read_process=READ_COMMAND_LAST;
      }else if(c==space){
        add_terminal(rc->key);
        rc->read_process=READ_EXPIRE_TIME;
      }else{
        add_char(rc->key,c);
      }
      break;
      case READ_EXPIRE_TIME:
      if(c==special_r){
        rc->read_process=READ_COMMAND_LAST;
      }
      break;
      case READ_COMMAND_LAST:
      if(c==special_n){
        rc->read_process=READ_COMMAND_END;
        return COMMAND_OK;
      }
      break;
      default:
      break;   
    }
  }
  return AGAIN;
}

static void process_stats(connection_t *conn){
  push(conn->wc->w_queue,get_buddy_stat(stat->small_buddy_stats,small_buddy));
  push(conn->wc->w_queue,get_buddy_stat(stat->big_buddy_stats,big_buddy));
  push(conn->wc->w_queue,get_direct_stat());
  get_queue_stat(conn);
  event_operation.register_event(conn->fd,WRITE,conn->ec,conn);
}

static item_t *get_buddy_stat(unsigned long *data,const char *type){
  item_t *i=init_item();
  i->data=alloc_mem(pool,STAT_LINE_BUFFER_SIZE);
  snprintf(i->data,STAT_LINE_BUFFER_SIZE,"buddy_mem-%s 0:%lu 1:%lu 2:%lu 3:%lu 4:%lu 5:%lu 6:%lu 7:%lu\n",type,*data,
    *(data+1),
    *(data+2),
    *(data+3),
    *(data+4),
    *(data+5),
    *(data+6),
    *(data+7)
    );
  i->end=1;
  i->c_size=strlen(i->data);
  return i;
}

static item_t *get_direct_stat(){
  item_t *i=init_item();
  i->data=alloc_mem(pool,STAT_LINE_BUFFER_SIZE);
  snprintf(i->data,STAT_LINE_BUFFER_SIZE,"buddy_mem-direct_size:%ld\n",stat->direct_size);
  i->end=1;
  i->c_size=strlen(i->data);
  return i;
}

static void get_queue_stat(connection_t *conn){
  list_head_t *head=visit_hash(stat->queue_data_hash);
  while(!list_is_empty(head)){
    hash_entry_t *entry=(hash_entry_t *)list_entry(head->next,hash_entry_t,list);
    queue_stat_t *qs=(queue_stat_t *)entry->data;
    item_t *i=init_item();
    i->data=alloc_mem(pool,STAT_LINE_BUFFER_SIZE);
    i->end=1;
    snprintf(i->data,STAT_LINE_BUFFER_SIZE,"queue_name:%s,size:%ld,bytes:%ld\n",qs->name,qs->size,qs->bytes);
    i->c_size=strlen(i->data);
    push(conn->wc->w_queue,i);
    free_mem(entry);
    list_del_data(head,head->next->next); 
  }
  free_mem(head);
}
