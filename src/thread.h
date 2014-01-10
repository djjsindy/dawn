#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
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
  struct kevent events[MAX_EVENT_COUNT]; 
};

typedef struct thread_s thread_t;

void start_workers();
