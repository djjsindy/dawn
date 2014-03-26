#include <stdlib.h>
#include "memory.h"
#include "item.h"

extern mem_pool_t *pool;

item_t* init_item(){
  item_t *i=(item_t *)alloc_mem(pool,sizeof(item_t));
  i->end=0;
  i->size=0;
  return i;
}

void destroy_item(item_t *i){
  free_mem(i->data);
  free_mem(i);
}
