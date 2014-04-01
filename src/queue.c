#include "queue.h"
#include "memory.h"
#include "my_log.h"
#include <stdlib.h>

extern mem_pool_t *pool;

queue_t *init_queue(){
  queue_t *q=(queue_t *)alloc_mem(pool,sizeof(queue_t));
  if(q==NULL){
    my_log(ERROR,"alloc queue failed\n");
    return NULL;
  }
  pthread_mutex_init(&(q->mutex), NULL);
  q->head=(list_head_t *)alloc_mem(pool,sizeof(list_head_t));
  if(q->head==NULL){
    my_log(ERROR,"alloc queue list head failed\n");
    return NULL;
  }
  init_list(q->head);
  return q;
}

int push(queue_t *q,void *data){
  pthread_mutex_lock(&(q->mutex));
  queue_item_t *item=(queue_item_t *)alloc_mem(pool,sizeof(queue_item_t));
  if(item==NULL){
    my_log(ERROR,"alloc queue item failed\n");
    return 0;
  }
  item->data=data;
  list_add_data(&(item->list),q->head,q->head->next);
  pthread_mutex_unlock(&(q->mutex));
  return 1;
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
  while(1){
    queue_item_t *i=pop(q);
    if(i==NULL){
      break;
    }
    free_mem(i->data);
    free_mem(i);
  }
  free_mem(q->head);
  free_mem(q);
}
