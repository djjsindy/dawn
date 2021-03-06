#ifndef CONNECTION_H
#define CONNECTION_H
#include <stdint.h>
#include "buffer.h"
#include "dy_char.h"
#include "network.h"
#include "queue.h"
#include "item.h"
//read status
#define AGAIN 0
#define COMMAND_OK 1
#define OK 2
//write status
#define ENOUGH 3

enum PARSE_STATUS{
  READ_COMMAND,
  READ_KEY,
  READ_FLAG,
  READ_EXPIRE_TIME,
  READ_BYTE_NUM,
  READ_COMMAND_LAST,
  READ_COMMAND_END,
 };

struct read_context_s{ 
  char_t *command;
  char_t *key;
  char_t *num;
  intptr_t last_bytes;
  enum PARSE_STATUS read_process;
};

typedef struct read_context_s read_context_t;

struct write_context_s{
  queue_t *w_queue;
  item_t *w_data;
  intptr_t w_index;
};

typedef struct write_context_s write_context_t;

struct connection_s{
  intptr_t fd;
  buffer_t *rbuf;
  buffer_t *wbuf;
  event_context_t *ec;
  write_context_t *wc;
  read_context_t *rc; 
  intptr_t events; 
};

typedef struct connection_s connection_t;


void reset_read_context(read_context_t *rc);

connection_t* init_connection(intptr_t fd);

void cancel_connection(connection_t *conn);
#endif


