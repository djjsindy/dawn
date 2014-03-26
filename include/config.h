#include "log.h"
struct parameter_s{
  int port=10000;
  int log_level=INFO;
  int back_log=50;
  int dynamic_char_size=16;
  int read_buf_size=4*1024;
  int write_buf_size=4*1024;
  int hash_size=4;
  float hash_factor=0.75f;
  int max_event_count=1024;
  int buddy_level=7;
  int worker_num=8;
};

typedef struct parameter_s parameter_t;


