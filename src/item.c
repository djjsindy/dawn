#include <stdlib.h>
#include <pthread.h>
#include "memory.h"
#include "item.h"

extern pthread_key_t key;

item_t* init_item(){
  mem_pool_t *pool=(mem_pool_t *)pthread_getspecific(key);
  item_t *i=(item_t *)alloc_mem(pool,sizeof(item_t));
  i->end=0;
  i->size=0;
  return i;
}

void destroy_item(item_t *i){
  free_mem(i->data);
  free_mem(i);
}
