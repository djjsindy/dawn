#ifndef THREAD_H
#define THREAD_H
#include "queue.h"
#include "connection.h"
#include "network.h"
#define WORKER_NUM 8

struct pipe_channel_s{
    int masterfd;
    int workerfd;
};

typedef struct pipe_channel_s pipe_channel_t;
struct thread_s{
    pipe_channel_t *pipe_channel;
    queue_t *queue;
};

typedef struct thread_s thread_t;

void start_workers();
 
void handle_notify(int fd,event_context_t *ec);

void handle_read(connection_t *conn);

int handle_write(connection_t *conn);

void accept_connection();

void close_connection(connection_t *conn);

#endif
