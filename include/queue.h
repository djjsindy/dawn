#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#include "list.h"
struct queue_item_s{
  void *data;
  list_head_t list;
};

typedef struct queue_item_s queue_item_t;

struct queue_s{
  list_head_t *head;
  int size;
  pthread_mutex_t mutex;
};

typedef struct queue_s queue_t;

queue_t* init_queue();

int push(queue_t *q,void *data);

void* pop(queue_t *q);

int queue_size(queue_t *q);

void destroy_queue(queue_t *q);
#endif
