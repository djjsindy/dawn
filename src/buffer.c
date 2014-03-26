#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "memory.h"
#include <pthread.h>

extern mem_pool_t *pool;

buffer_t* alloc_buffer(int size){
  buffer_t *b=(buffer_t *)alloc_mem(pool,sizeof(buffer_t));
  if(b==NULL){
    printf("alloc buffer error");
    exit(1);
  }
  b->data=(char *)alloc_mem(pool,size);
  if(b->data==NULL){
    printf("alloc buffer error");
    exit(1);
  }
  b->size=size;
  b->current=0;
  b->limit=0;
  return b;
}

void compact(buffer_t *b){
  memset(b->data,0,b->current);
  memcpy(b->data,b->data+b->current,b->limit-b->current);
  b->limit-=b->current;
  b->current=0;
}

void reset(buffer_t *b){
  memset(b->data,0,b->size);
  b->current=0;
  b->limit=0;
}

int has_space(buffer_t *b){
  return b->limit<b->size;
}

int has_remaining(buffer_t *b){
  return b->current<b->limit;
}

void destroy_buffer(buffer_t *b){
  free_mem(b->data);
  free_mem(b);
}
