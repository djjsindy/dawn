#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "memory.h"
#include "dy_char.h"
#include "my_log.h"

extern mem_pool_t *pool;

void add_terminal(char_t *t);

static void expand_char(char_t *t);

char_t *init_char(){
  char_t *t=(char_t *)alloc_mem(pool,sizeof(char_t));
  if(t==NULL){
    my_log(ERROR,"alloc dynamic char struct failed\n");
    return NULL;
  }
  t->data=(char *)alloc_mem(pool,CHAR_SIZE);
  if(t->data==NULL){
    my_log(ERROR,"alloc dynamic char data failed\n");
    return NULL;
  }
  t->current=0;
  t->size=CHAR_SIZE;
  return t;
}

void add_char(char_t *t,char c){
  if(t->current==t->size){
    expand_char(t);
  }
  *(t->data+t->current)=c;
  t->current+=1;
}

void add_terminal(char_t *t){
  if(t->current==t->size){
    expand_char(t);
  }
  *(t->data+t->current)='\0';
  t->current+=1;
}
static void expand_char(char_t *t){
  int size=t->size;
  char *temp=(char *)alloc_mem(pool,size*2);
  if(temp==NULL){
    my_log(ERROR,"expand dynamic char failed\n");
  }
  memcpy(temp,t->data,size);
  free_mem(t->data);
  t->data=temp;
  t->size*=2;
}

void reset_char(char_t *t){
  memset(t->data,0,t->current);
  t->current=0;
}

void destroy_char(char_t *t){
  free_mem(t->data);
  free_mem(t);
}
