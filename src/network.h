#ifndef NETWORK_H
#define NETWORK_H
#include "thread.h"
struct event_context_s{
  int fd;
  void *events;
  int listen_fd;
};

typedef struct event_context_s event_context_t;

enum EVENT{READ,WRITE};

struct event_operation_s{
   void (*init_event)(event_context_t *ec);

   void (*register_event)(int fd,enum EVENT event,thread_t *t,void *data);

   void (*add_listen_event)(event_context_t *ec);

   void (*del_event)(int fd,enum EVENT event);
   
   void (*process_listen_event)(event_context_t *ec);

   void (*process_event)(event_context_t *ec,thread_t *t);
};

typedef struct event_operation_s event_operation_t;

event_operation_t event_operation;

#endif
