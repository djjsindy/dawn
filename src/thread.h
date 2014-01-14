#ifndef THREAD_H
#define THREAD_H
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "network.h"
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
};

typedef struct thread_s thread_t;

void start_workers();
 
void handle_notify(int fd,event_context_t *ec,thread_t *t);

void handle_read(int fd);

void handle_write(int fd);
#endif
