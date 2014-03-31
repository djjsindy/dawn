#include "queue.h"
#include "memory.h"
#include <stdlib.h>

extern mem_pool_t *pool;

queue_t *init_queue(){
  queue_t *q=(queue_t *)alloc_mem(pool,sizeof(queue_t));
  pthread_mutex_init(&(q->mutex), NULL);
  q->head=(list_head_t *)alloc_mem(pool,sizeof(list_head_t));
  init_list(q->head);
  return q;
}

void push(queue_t *q,void *data){
  pthread_mutex_lock(&(q->mutex));
  queue_item_t *item=(queue_item_t *)alloc_mem(pool,sizeof(queue_item_t));
  item->data=data;
  list_add_data(&(item->list),q->head,q->head->next);
  pthread_mutex_unlock(&(q->mutex));
}

void* pop(queue_t *q){
  void *data=NULL;
  pthread_mutex_lock(&(q->mutex));
  if(!list_is_empty(q->head)){
    list_head_t *del=list_del_data(q->head->prev->prev,q->head);
    queue_item_t *item=list_entry(del,queue_item_t,list);
    data=item->data;
    free_mem(item);  
  }  
  pthread_mutex_unlock(&(q->mutex));
  return data;
}

void destroy_queue(queue_t *q){
  free_mem(q);
}
