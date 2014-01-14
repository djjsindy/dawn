#ifndef NETWORK_H
#define NETWORK_H
struct event_context_s{
  int fd;
  void *events;
  int listen_fd;
};

typedef struct event_context_s event_context_t;

struct event_operation_s{
   void (*init_event)(event_context_t *ec);

   void (*register_event)(int fd,int flag,event_context_t *ec,void *data);

   void (*add_listen_event)(event_context_t *ec);

   void (*del_event)(int fd,int flag);
   
   void (*process_listen_event)(event_context_t *ec);

   void (*process_event)(event_context_t *ec);
};

typedef struct event_operation_s event_operation_t;

#endif
