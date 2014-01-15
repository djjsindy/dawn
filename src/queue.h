#ifndef QUEUE_H
#define QUEUE_H
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct queue_item_s{
  void *data;
  struct queue_item_s *next;
};

typedef struct queue_item_s queue_item_t;

struct queue_s{
  queue_item_t *head;
  queue_item_t *tail;
  pthread_mutex_t *mutex;
};

typedef struct queue_s queue_t;

queue_t* init_queue();

void push(queue_t *q,void *data);

void* pop(queue_t *q);
#endif
