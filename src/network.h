#ifndef NETWORK_H
#define NETWORK_H
#include "list.h"
#include "queue.h"
struct event_context_s{
  int fd;
  void *events;
  int listen_fd;
  int worker_fd;
  queue_t *queue;
};

typedef struct event_context_s event_context_t;

enum EVENT{READ,WRITE};

struct event_operation_s{
   void (*init_event)(event_context_t *ec);

   void (*register_event)(int fd,enum EVENT event,event_context_t *ec,void *data);

   void (*add_listen_event)(event_context_t *ec);

   void (*del_event)(int fd,enum EVENT event,event_context_t *ec);
   
   void (*process_listen_event)(event_context_t *ec);

   void (*process_event)(event_context_t *ec);
};

typedef struct event_operation_s event_operation_t;

event_operation_t event_operation;

#endif
