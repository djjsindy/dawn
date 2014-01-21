#include <stdio.h>
#include "buffer.h"

buffer_t* alloc_buffer(int size){
  buffer_t *b=(buffer_t *)malloc(sizeof(buffer_t));
  if(b==NULL){
    printf("alloc buffer error");
    exit(1);
  }
  b->data=(char *)malloc(size);
  if(b->data==NULL){
    printf("alloc buffer error");
    exit(1);
  }
  b->size=size;
  b->current=0;
  b->limit=0;
}

void compact(buffer_t *t){
  memset(t->data,t->data+current,t->limit-t->current);
  t->limit-=t->current;
  t->current=0;
}
