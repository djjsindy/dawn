#include <stdlib.h>
#include "item.h"

item_t* init_item(){
  item_t *i=(item_t *)malloc(sizeof(item_t));
  i->end=0;
  i->size=0;
  return i;
}

void destroy_item(item_t *i){
  free(i->data);
  free(i);
}
