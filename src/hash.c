#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "memory.h"
#include "hash.h"
#include "my_log.h"
#include "config.h"

extern mem_pool_t *pool;

static int expand_hash(hash_t* hash);

static unsigned long cal_hash(char *k);

static void set_init_size_value(char_t *value);

static void set_factor_value(char_t *value);

static int init_size=INIT_SIZE;

static float hash_factor=HASH_FACTOR;

/**
 * 初始化hash结构，包括锁，初始化elements空间
 */
hash_t* init_hash(){
  hash_t *h=(hash_t*)alloc_mem(pool,sizeof(hash_t));
  if(h==NULL){
    my_log(ERROR,"alloc hash struct failed\n");
  }
  h->elements=(hash_element_t **)alloc_mem(pool,sizeof(hash_element_t *)*init_size);
  if(h->elements==NULL){
    my_log(ERROR,"alloc hash elements failed\n");
  }
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  h->mutex=&mutex;
  h->size=init_size;
  h->num=0;
  return h;
}

/**
 * 向hash结构中put数据
 */
int put(char *k,void *data,hash_t *hash){
  pthread_mutex_lock(hash->mutex);
  void *d=get(k,hash);
  if(d!=NULL){
    return 0;
  }
  float factor=hash->num/hash->size;

  /**
   * 如果当前hash中元素的数量和hash array的空间之比大于factor，那么对hash进行扩容
   */

  if(factor>hash_factor){
    if(!expand_hash(hash)){
      return 0;
    }
  }
  int index=cal_hash(k)%hash->size;
  hash_element_t *nelement=(hash_element_t *)alloc_mem(pool,sizeof(hash_element_t));
  if(nelement==NULL){
    my_log(ERROR,"alloc hash element failed\n");
    return 0;
  }
  nelement->key=k;
  nelement->data=data;
  hash_element_t *next=hash->elements[index];
  hash->elements[index]=nelement;
  nelement->next=next;
  hash->num+=1;
  pthread_mutex_unlock(hash->mutex);
  return 1;
}

/**
 * 从hash结构中，获取key所对应的value的值
 */
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

/**
 *  从hash表中删除key的数据
 */
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

/**
 *  扩容hash结构，当存储元素的个数大于size*factor的时候需要expand，避免退化链表结构，每次扩容2倍的size
 */
static int expand_hash(hash_t *hash){
  hash_element_t **temp=(hash_element_t **)alloc_mem(pool,sizeof(hash_element_t *)*(hash->size*2));
  if(temp==NULL){
    my_log(ERROR,"expand hash data failed\n");
    return 0;
  }
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
  return 1;
}


/**
 *  hash函数，计算key值的hash，现在的做法是把每个char叠加在一起，后面需要改善
 */
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

/**
 * 遍历整个hash结构，返回list，针对每个元素，使用完成后，必须free_mem
 */
list_head_t* visit_hash(hash_t *hash){
  pthread_mutex_lock(hash->mutex);
  list_head_t* head=(list_head_t *)alloc_mem(pool,sizeof(list_head_t));
  init_list(head);
  int index=0;
  for(;index<hash->size;index++){
    hash_element_t *element=hash->elements[index];
    while(element!=NULL){
      hash_entry_t *entry=(hash_entry_t *)alloc_mem(pool,sizeof(hash_entry_t));
      entry->data=element->data;
      entry->key=element->key;
      list_add_data(&(entry->list),head,head->next);
      element=element->next;
    }
  }
  pthread_mutex_unlock(hash->mutex);
  return head;
}

static command_t hash_command[]={
  {"init_size",set_init_size_value},
  {"factor",set_factor_value},
  NULL
};

config_module_t hash_conf_module={
  "hash",
  hash_command
};

static void set_init_size_value(char_t *value){
  init_size=atoi(value->data);
}

static void set_factor_value(char_t *value){
  hash_factor=atof(value->data);
}

