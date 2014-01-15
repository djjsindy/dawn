#ifndef THREAD_H
#define THREAD_H

#include "queue.h"
#include "connection.h"

#define WORKER_NUM 8
#define MAX_EVENT_COUNT 1024
#define NOTIFY_BUF_SIZE 32

struct pipe_channel_s{
  int masterfd;
  int workerfd;
};

typedef struct pipe_channel_s pipe_channel_t;
struct thread_s{
  pipe_channel_t *pipe_channel;
  int kqueuefd;
  queue_t *newconn;
};

typedef struct thread_s thread_t;

void start_workers();
 
void handle_notify(int fd,thread_t *t);

void handle_read(connection_t *conn);

void handle_write(connection_t *conn);
#endif
