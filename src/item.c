#include <stdlib.h>
#include "memory.h"
#include "item.h"
#include "my_log.h"

extern mem_pool_t *pool;

item_t* init_item(){
  item_t *i=(item_t *)alloc_mem(pool,sizeof(item_t));
  if(i==NULL){
  	my_log(ERROR,"alloc item error\n");
  	return NULL;
  }
  i->end=0;
  i->size=0;
  i->c_size=0;
  return i;
}

void destroy_item(item_t *i){
  free_mem(i->data);
  free_mem(i);
}
