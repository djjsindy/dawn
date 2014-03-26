#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "memory.h"
#include "hash.h"
#define INIT_SIZE 4

extern mem_pool_t *pool;

static void expand_hash(hash_t* hash);

static unsigned long cal_hash(char *k);

static void* none_lock_get(char *key,hash_t *hash);

hash_t* init_hash(){
  hash_t *h=(hash_t*)alloc_mem(pool,sizeof(hash_t));
  if(h==NULL){
    printf("alloc hash error\n");
    exit(0);
  }
  h->elements=(hash_element_t **)alloc_mem(pool,sizeof(hash_element_t *)*INIT_SIZE);
  if(h->elements==NULL){
    printf("alloc hash elements error\n");
    exit(0);
  }
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  h->mutex=&mutex;
  h->size=INIT_SIZE;
  h->num=0;
  return h;
}

void put(char *k,void *data,hash_t *hash){
  pthread_mutex_lock(hash->mutex);
  void *d=none_lock_get(k,hash);
  if(d!=NULL){
    return;
  }
  float factor=hash->num/hash->size;
  if(factor>0.75f){
    expand_hash(hash);
  }
  int index=cal_hash(k)%hash->size;
  hash_element_t *nelement=(hash_element_t *)alloc_mem(pool,sizeof(hash_element_t));
  if(nelement==NULL){
    printf("alloc new element error\n");
    exit(0);
  }
  nelement->key=k;
  nelement->data=data;
  hash_element_t *next=hash->elements[index];
  hash->elements[index]=nelement;
  nelement->next=next;
  hash->num+=1;
  pthread_mutex_unlock(hash->mutex);
}

void* get(char* key,hash_t *hash){
  pthread_mutex_lock(hash->mutex);
  int index=cal_hash(key)%hash->size;
  hash_element_t *e=hash->elements[index];
  void *result=NULL;
  while(e!=NULL){    
    if(strcmp(e->key,key)==0){
      result=e->data;
      break;
    }
    e=e->next;
  }
  pthread_mutex_unlock(hash->mutex);
  return result;
}

static void* none_lock_get(char *key,hash_t *hash){
  int index=cal_hash(key)%hash->size;
  hash_element_t *e=hash->elements[index];
  void *result=NULL;
  while(e!=NULL){
    if(strcmp(e->key,key)==0){
      result=e->data;
      break;
    }
    e=e->next;
  }
  return result;
}

void delete(char* key,hash_t *hash){ 
  pthread_mutex_lock(hash->mutex);
  int index=cal_hash(key)%hash->size;
  hash_element_t *e=hash->elements[index];
  hash_element_t *prev;
  while(e!=NULL){
    if(strcmp(e->key,key)==0){
      break;
    }
    prev=e;
    e=e->next;
  }
  if(prev==NULL){
    hash->elements[index]=e->next;
  }else{
    prev->next=e->next;
  }
  pthread_mutex_unlock(hash->mutex);
}

static void expand_hash(hash_t *hash){
  hash_element_t **temp=(hash_element_t **)alloc_mem(pool,sizeof(hash_element_t *)*(hash->size*2));
  int index=0;
  for(;index<hash->size;index++){
    hash_element_t *element=hash->elements[index];
    while(element!=NULL){
      int i=cal_hash(element->key)%(hash->size*2);
      element->next=temp[i];
      temp[i]=element;
      element=element->next; 
    }
  }
  free_mem(hash->elements);
  hash->elements=temp;
  hash->size*=2;
}

static unsigned long cal_hash(char *key){
  int index=0;
  int length=(int)strlen(key);
  unsigned long hash;
  while(index<length){
    hash+=*(key+index);
    index++;
  }
  return hash;
}

