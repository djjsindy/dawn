#include "buffer.h"
#include "dy_char.h"
static void process_set(buffer_t *t,connection_t *conn);

static void process_get(buffer_t *t,connection_t *conn);

int parse_command(buffer_t *buf,connection_t *conn){
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
        if(c=' '){
          conn->read_process=READ_BYTE_NUM;
        }
        break;
      case READ_BYTE_NUM:
        if(c=='\r'){
          conn->read_process=READ_COMMAND_LAST;
        }else{
          add_char(conn->num,c);
        }
        break;
      case READ_COMMAND_LAST:
        if(c='\n'){
          conn->read_process=READ_COMMAND_END;
          return 1;
        }
        break;
    }
    buf->current+=1;
  }
  compact(buf);
  return 0;
}

void process_command(buffer_t *b,connection_t *conn){
  switch(conn->command->data){
    case "set":
      process_set(b,conn);
      break;
    case "get":
      process_get(b,conn);
  }
}

static void process_set(buffer_t *t,connection_t *conn){
  
}
