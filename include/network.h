#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
#include "list.h"
#include "queue.h"

#define MAX_EVENT_COUNT 1024

#define TIMEOUT 1

struct event_context_s{
  #ifdef HAVE_EVENT
    intptr_t fd;
  #endif
  
  #ifdef HAVE_EPOLL
    intptr_t fd;
  #endif

  intptr_t listen_fd;
  intptr_t worker_fd;
  queue_t *queue;
  void *events;
  #ifdef HAVE_POLL
    void *data;
  #endif
  
};

typedef struct event_context_s event_context_t;

enum EVENT{
  READ=1,WRITE=2
};

struct event_operation_s{
   void (*init_event)(event_context_t *ec);

   void (*register_event)(intptr_t fd,enum EVENT event,event_context_t *ec,void *data);

   void (*del_event)(intptr_t fd,enum EVENT event,event_context_t *ec,void *data);

   void (*process_event)(event_context_t *ec);

   void (*close_event)(intptr_t fd,event_context_t *ec);
};

typedef struct event_operation_s event_operation_t;

event_operation_t event_operation;

#endif
