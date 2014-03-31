#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "memory.h"
static void* get_available(list_head_t **bin,int size,int bin_size,mem_pool_t *pool);

static int bin_index(int size,int bin_size);

static buddy_t* init_buddy(int size,mem_pool_t *pool);

static void* get_buddy_mem(buddy_t *buddy,int level);

static void update_buddy_max(buddy_t *buddy);

static void adjust_bin(buddy_t *buddy,int origin_index);

static void* direct_alloc(list_head_t *head,int size,mem_pool_t *pool);

static void free_buddy_chunk(mem_buddy_chunk_t *chunk);

static void update_child_flag(buddy_t *buddy,int index,int flag);

static void update_buddy_flag_inuse(buddy_t *buddy,int index);

static void update_buddy_flag_unuse(buddy_t *buddy,int index);

static void free_buddy_bin(list_head_t **bin);

static void free_direct_list(list_head_t *head);

static int decide_real_size(int size);

static void init_pool_list(list_head_t **base,int num);

static void init_pthread_mutex(mem_pool_t *pool);


mem_pool_t* init_mem_pool(){
  int num=DEFAULT_LEVEL+1;
  mem_pool_t* pool=(mem_pool_t *)malloc(sizeof(mem_pool_t));
  pool->small_bin=(list_head_t **)malloc(sizeof(list_head_t *)*(num));
  init_pool_list(pool->small_bin,num);
  pool->big_bin=(list_head_t **)malloc(sizeof(list_head_t *)*(num));
  init_pool_list(pool->big_bin,num);
  init_list(&(pool->direct_head));
  init_pthread_mutex(pool);
  return pool;
}

static void init_pthread_mutex(mem_pool_t *pool){
  pthread_mutex_init(&(pool->small_mutex), NULL);
  pthread_mutex_init(&(pool->big_mutex), NULL);
  pthread_mutex_init(&(pool->direct_mutex), NULL);
}

static void init_pool_list(list_head_t **base,int num){
  int index=0;
  for(;index<num;){
    list_head_t *head=(list_head_t *)malloc(sizeof(list_head_t));
    init_list(head);
    *(base+index)=head;
    index++;
  }
}

void destroy_mem_pool(mem_pool_t *pool){
  free_buddy_bin(pool->small_bin);
  free(pool->small_bin); 
  free_buddy_bin(pool->big_bin);
  free(pool->big_bin);
  free_direct_list(&(pool->direct_head));
  pthread_mutex_destroy(&(pool->small_mutex));
  pthread_mutex_destroy(&(pool->big_mutex));
  pthread_mutex_destroy(&(pool->direct_mutex));
  free(pool);
}

static void free_direct_list(list_head_t *head){
  list_head_t *next=head->next;
  while(next!=head){
    mem_direct_chunk_t *chunk=(mem_direct_chunk_t *)next;
    next=chunk->list.next;
    free(chunk);
  }
}

static void free_buddy_bin(list_head_t **bin){
  int index=0;
  while(index<=DEFAULT_LEVEL){
    list_head_t *head=(list_head_t *)*(bin+index);
    if(!list_is_empty(head)){
      list_head_t *next=head->next;
      while(next!=head){
        buddy_t *buddy=(buddy_t *)next;
        next=buddy->list.next;
        free(buddy->base);
        free(buddy);
      }
    }
    index++;
    free(head);
  }
}

void* alloc_mem(mem_pool_t *pool,int size){
  int real_size=decide_real_size(size);
  void *p;
  if(real_size<=SMALL_THRESHOLD){
    pthread_mutex_lock(&(pool->small_mutex));
    p=get_available(pool->small_bin,real_size,SMALL_THRESHOLD,pool);
    pthread_mutex_unlock(&(pool->small_mutex));
  }else if(real_size<=BIG_THRESHOLD){
    pthread_mutex_lock(&(pool->big_mutex));
    p=get_available(pool->big_bin,real_size,BIG_THRESHOLD,pool);
    pthread_mutex_unlock(&(pool->big_mutex));
  }else{
    pthread_mutex_lock(&(pool->direct_mutex));
    p=direct_alloc(&(pool->direct_head),real_size,pool);
    pthread_mutex_unlock(&(pool->direct_mutex));
  }
  return p;
}

static int decide_real_size(int size){
  int buddy_s_size=sizeof(mem_buddy_chunk_t);
  int direct_s_size=sizeof(mem_direct_chunk_t);
  return size+(buddy_s_size>direct_s_size?buddy_s_size:direct_s_size);
}

static buddy_t* init_buddy(int size,mem_pool_t *pool){

  buddy_t *buddy=(buddy_t *)malloc(sizeof(buddy_t));
  int i=0;
  for(;i<sizeof(buddy->flags)/sizeof(int);i++){
    buddy->flags[i]=1;
  }
  void *mem=malloc(size);
  buddy->base=mem;
  buddy->size=size;
  buddy->max=size;
  buddy->pool=pool;
  return buddy;
}

static void* get_buddy_mem(buddy_t *buddy,int level){
  int size=buddy->size;
  int start=LEVEL_START(level);
  int end=LEVEL_END(level);
  int index=start;
  mem_buddy_chunk_t *chunk;
  while(index<=end){
    if(buddy->flags[index]==1){
      chunk=buddy->base+((buddy->size)>>level)*(index-start);
      break;
    }
    index++;
  }
  chunk->buddy=buddy;
  chunk->is_direct=0;
  chunk->level=level;
  chunk->pool=buddy->pool;
  update_buddy_flag_inuse(buddy,index);
  update_buddy_max(buddy);
  return USER_BUDDY_MEM(chunk);
}


static void update_buddy_flag_unuse(buddy_t *buddy,int index){
  int parent;
  int source=index;
  while(index>0&&(parent=PARENT(index))>=0){
    if(buddy->flags[parent]==1){
      break;
    }else{
      if(LEFT_CHILD(parent)==index&&buddy->flags[RIGHT_CHILD(parent)]==1&&buddy->flags[parent]==0){
        buddy->flags[parent]=1;
      }else if(RIGHT_CHILD(parent)==index&&buddy->flags[LEFT_CHILD(parent)]==1&&buddy->flags[parent]==0){
        buddy->flags[parent]=1;
      }else{
        break;
      }
    }
    index=parent;
  }
  update_child_flag(buddy,source,1);
}

static void update_buddy_flag_inuse(buddy_t *buddy,int index){

  /**
   * 更新parent flag
   */
  int parent=PARENT(index);
  while(parent>=0){
    if(buddy->flags[parent]==0){
      break;
    }else{    
      buddy->flags[parent]=0;
      parent=PARENT(parent);
    }
  }
  /**
   * 更新child flag
   */
  update_child_flag(buddy,index,0);
}

static void update_child_flag(buddy_t *buddy,int index,int flag){  
  if(index>sizeof(buddy->flags)/sizeof(int)-1){
    return;
  }
  buddy->flags[index]=flag;
  update_child_flag(buddy,LEFT_CHILD(index),flag);
  update_child_flag(buddy,RIGHT_CHILD(index),flag);
}

static void update_buddy_max(buddy_t *buddy){
  int level=0;
  while(level<DEFAULT_LEVEL){
    int start=LEVEL_START(level);
    int end=LEVEL_END(level);
    while(start<=end){
      if(buddy->flags[start]!=0){
        buddy->max=buddy->size>>level;
        return;
      }
      start++;
    }
    level++;
  }
  buddy->max=0;
}

static int bin_index(int size,int bin_size){
  if(size==0)
    return 0;
  assert(bin_size>=size);
  int i=DEFAULT_LEVEL;
  while(i>0){
    if(size==bin_size){
      return i;
    }else if(size>bin_size){
      return i+1;
    }
    bin_size/=2;
    i--;
  }
  return 0;
}

static void* get_available(list_head_t **bin,int size,int bin_size,mem_pool_t *pool){
  int level=bin_index(size,bin_size);
  buddy_t *buddy=NULL;
  int test=level;
  while(test<=DEFAULT_LEVEL){
    list_head_t *head=(list_head_t *)*(bin+test);
    if(!list_is_empty(head)){
      buddy=list_entry(head->next,buddy_t,list);
      break;
    }
    test++;
  }
  if(buddy==NULL){
    list_head_t *head=*(bin+level);
    buddy=init_buddy(bin_size,pool);
    buddy->bin=bin;
    list_add_data(&buddy->list,head,head->next);
  }
  int max=buddy->max;
  void* result=get_buddy_mem(buddy,DEFAULT_LEVEL-level);
  if(max!=buddy->max){
    adjust_bin(buddy,level);
  }
  return result;
}

static void adjust_bin(buddy_t *buddy,int index){
  //delete from origin buddy list
  list_del_data(buddy->list.prev,buddy->list.next);
  //add buddy to new list
  int new_index=bin_index(buddy->max,buddy->size);
  list_head_t *head=*(buddy->bin+new_index);
  list_add_data(&buddy->list,head,head->next);
}

static void* direct_alloc(list_head_t *list, int size,mem_pool_t *pool){
  mem_direct_chunk_t *chunk=(mem_direct_chunk_t *)malloc(size);
  chunk->is_direct=1;
  chunk->pool=pool;
  list_add_data(&chunk->list,list,list->next);
  return USER_DIRECT_MEM(chunk);
}

void free_mem(void *mem){
  int flag=*(FLAG_MEM(mem));
  if(flag==1){
    mem_direct_chunk_t *chunk=CHUNK_DIRECT_MEM(mem);
    pthread_mutex_lock(&(chunk->pool->direct_mutex));
    list_del_data(chunk->list.prev,chunk->list.next); 
    free(&(chunk->list));
    free(chunk);
    pthread_mutex_unlock(&(chunk->pool->direct_mutex));
  }else{
    mem_buddy_chunk_t *chunk=CHUNK_BUDDY_MEM(mem);
    free_buddy_chunk(chunk);
  }
}

static void free_buddy_chunk(mem_buddy_chunk_t *chunk){
  buddy_t *buddy=chunk->buddy;
  int max=buddy->max;
  pthread_mutex_t *mutex;
  if(max==SMALL_THRESHOLD){
    mutex=&(chunk->pool->small_mutex);
  }else{
    mutex=&(chunk->pool->big_mutex);
  }
  pthread_mutex_lock(mutex);
  int origin_index=bin_index(max,buddy->size);
  int diff=((void*)chunk - buddy->base)/(buddy->size>>chunk->level);
  int index=LEVEL_START(chunk->level)+diff;
  update_buddy_flag_unuse(buddy,index);  
  update_buddy_max(buddy);
  if(max!=buddy->max){
    adjust_bin(buddy,origin_index);
  }
  pthread_mutex_unlock(mutex);
}
