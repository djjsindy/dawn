#include "queue.h"
#include <stdlib.h>
queue_t *init_queue(){
  queue_t *q=(queue_t *)malloc(sizeof(queue_t));
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  q->mutex=&mutex;
  q->head=NULL;
  q->tail=NULL;
  return q;
}

void detroy_queue(queue_t *t){
}

void push(queue_t *q,void *data){
  pthread_mutex_lock(q->mutex);
  queue_item_t *item=(queue_item_t *)malloc(sizeof(queue_item_t));
  item->data=data;
  if(q->tail==NULL&&q->head==NULL){
    q->tail=item;
    q->head=item;
  }else if(q->tail==q->head){  
    q->head->next=item;
    q->tail=item;
  }else{
    queue_item_t *tail=q->tail;
    tail->next=item;
    q->tail=item;
  }
  pthread_mutex_unlock(q->mutex);
}

void* pop(queue_t *q){
  pthread_mutex_lock(q->mutex);
  if(q->head==NULL){
    return 0;
  }
  void *data=q->head->data;
  queue_item_t *t=q->head;
  if(q->head==q->tail){
    q->head=NULL;
    q->tail=NULL;
  }else{
    q->head=q->head->next;
  }
  free(t);
  pthread_mutex_unlock(q->mutex);
  return data;
}


