#include "queue.h"
#include "memory.h"
#include <pthread.h>
#include <stdlib.h>

extern pthread_key_t key;

queue_t *init_queue(){
  mem_pool_t *pool=(mem_pool_t *)pthread_getspecific(key);
  queue_t *q=(queue_t *)alloc_mem(pool,sizeof(queue_t));
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
  mem_pool_t *pool=(mem_pool_t *)pthread_getspecific(key);
  queue_item_t *item=(queue_item_t *)alloc_mem(pool,sizeof(queue_item_t));
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
  free_mem(t);
  pthread_mutex_unlock(q->mutex);
  return data;
}

void destroy_queue(queue_t *q){
  free_mem(q);
}
