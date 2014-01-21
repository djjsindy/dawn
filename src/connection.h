#ifndef CONNECTION_H
#define CONNECTION_H
#include "buffer.h"
#include "dy_char.h"
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
struct connection_s{
  int fd;
  buffer_t *rbuf;
  buffer_t *wbuf;
  char_t *command;
  char_t *key;
  char_t *num;
  enum PARSE_STATUS read_process;
};

typedef struct connection_s connection_t;
#endif

enum PARSE_STATUS{
  READ_COMMAND;
  READ_KEY;
  READ_FLAG;
  READ_EXPIRE_TIME;
  READ_BYTE_NUM;
  READ_COMMAND_LAST;
  READ_COMMAND_END;
};
