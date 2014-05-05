#ifndef THREAD_H
#define THREAD_H
#include <stdint.h>
#include "queue.h"
#include "connection.h"
#include "network.h"

struct pipe_channel_s{
    intptr_t masterfd;
    intptr_t workerfd;
};

typedef struct pipe_channel_s pipe_channel_t;
struct thread_s{
    pipe_channel_t *pipe_channel;
    queue_t *queue;
};

typedef struct thread_s thread_t;

void start_workers();
 
void handle_notify(intptr_t fd,event_context_t *ec);

void handle_read(connection_t *conn);

intptr_t handle_write(connection_t *conn);

void accept_connection();

void close_connection(connection_t *conn);

#endif
