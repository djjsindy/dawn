#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "memory.h"
#include "dy_char.h"

extern mem_pool_t *pool;

void add_terminal(char_t *t);

static void expand_char(char_t *t);

char_t *init_char(int size){
  char_t *t=(char_t *)alloc_mem(pool,sizeof(char_t));
  if(t==NULL){
    printf("alloc char error\n");
    exit(0);
  }
  t->data=(char *)alloc_mem(pool,size);
  if(t->data==NULL){
    printf("alloc char data error\n");
    exit(0);
  }
  t->current=0;
  t->size=size;
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
  memcpy(temp,t->data,size);
  free_mem(t->data);
  t->data=temp;
}

void reset_char(char_t *t){
  memset(t->data,0,strlen(t->data));
  t->size=0;
  t->current=0;
}


void add_chars(char_t *t,char *cs){
  int size=sizeof(cs);
  int index=0;
  while(index<size){
    add_char(t,*(cs+index));
    index++;
  }
}

void destroy_char(char_t *t){
  free_mem(t->data);
  free_mem(t);
}
