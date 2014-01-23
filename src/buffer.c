#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  return b;
}

void compact(buffer_t *t){
  memset(t->data,0,t->current);
  memcpy(t->data,t->data+t->current,t->limit-t->current);
  t->limit-=t->current;
  t->current=0;
}

void reset(buffer_t *t){
  memset(t->data,0,t->size);
  t->current=0;
  t->limit=0;
}

int has_space(buffer_t *t){
  return t->limit<t->size;
}
