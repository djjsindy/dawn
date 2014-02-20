#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "memory.h"
static void* get_available(void **bin,int size,int bin_size);

static int bin_index(int size,int bin_size);

static buddy_t* init_buddy(int size);

static void* get_buddy_mem(buddy_t *buddy,int level);

static void update_buddy_max(buddy_t *buddy);

static void adjust_bin(buddy_t *buddy,int origin_index);

static void* direct_alloc(int size);

static void free_chunk(mem_chunk_t *chunk);

static void update_child_flag(buddy_t *buddy,int index,int flag);

static void update_buddy_flag_inuse(buddy_t *buddy,int index,int flag);

static void update_buddy_flag_unuse(buddy_t *buddy,int index,int flag);

mem_pool_t* init_mem_pool(){
  mem_pool_t* pool=(mem_pool_t *)malloc(sizeof(mem_pool_t));
  pool->small_bin=(void **)malloc(sizeof(void*)*(DEFAULT_LEVEL+1));
  pool->big_bin=(void **)malloc(sizeof(void*)*(DEFAULT_LEVEL+1));
  return pool;
}


void* alloc_mem(mem_pool_t *pool,int size){
  int real_size=size+sizeof(mem_chunk_t);
  if(real_size<=SMALL_THRESHOLD){
    return get_available(pool->small_bin,size,SMALL_THRESHOLD);
  }else if(real_size<=BIG_THRESHOLD){
    return get_available(pool->big_bin,size,BIG_THRESHOLD);
  }else{
    return direct_alloc(size);
  }
}

static buddy_t* init_buddy(int size){
  buddy_t *buddy=(buddy_t *)malloc(sizeof(buddy_t));
  int i=0;
  for(;i<sizeof(buddy->flags)/sizeof(int);i++){
    buddy->flags[i]=1;
  }
  void *mem=malloc(size);
  buddy->base=mem;
  buddy->size=size;
  return buddy;
}

static void* get_buddy_mem(buddy_t *buddy,int level){
  int size=buddy->size;
  int start=pow(2,level)-1;
  int end=pow(2,level+1)-2;
  int index=start;
  mem_chunk_t *chunk;
  while(index<=end){
    if(buddy->flags[index]==1){
      chunk=buddy->base+((buddy->size)>>level)*(index-start);
      break;
    }
    index++;
  }
  assert(chunk!=NULL);
  chunk->buddy=buddy;
  chunk->is_direct=0;
  chunk->level=level;
  update_buddy_flag_inuse(buddy,index,-1);
  update_buddy_max(buddy);
  return USER_MEM(chunk);
}
static void update_buddy_flag_unuse(buddy_t *buddy,int index,int flag){
  int parent;
  int source=index;
  while((parent=(index-1)>>1)>=0){
    if(buddy->flags[parent]==flag){
      break;
    }else{
      if(parent*2+1==index&&buddy->flags[parent*2+2]==flag){
        buddy->flags[parent]=flag;
      }else if(parent*2+2==index&&buddy->flags[parent*2+1]==flag){
        buddy->flags[parent]=flag;
      }else{
        break;
      }
    }
    index=parent;
  }
  update_child_flag(buddy,source,flag);
}

static void update_buddy_flag_inuse(buddy_t *buddy,int index,int flag){

  //update parent flag
  int parent=(index-1)>>1;
  while(parent>=0){
    if(buddy->flags[parent]==-flag){
      break;
    }else{    
      buddy->flags[parent]=flag;
      parent=(parent-1)>>1;
    }
  }
  //update child flag
  update_child_flag(buddy,index,flag);
}

static void update_child_flag(buddy_t *buddy,int index,int flag){  
  if(index>sizeof(buddy->flags)/sizeof(int)-1){
    return;
  }
  buddy->flags[index]=flag;
  update_child_flag(buddy,index*2+1,flag);
  update_child_flag(buddy,index*2+2,flag);
}

static void update_buddy_max(buddy_t *buddy){
  int i=0;
  while(i<=DEFAULT_LEVEL){
    int j=pow(2,i)-1;
    int k=pow(2,i+1)-1;
    while(j<k){
      if(buddy->flags[j]!=-1){
        buddy->max=buddy->size>>i;
        return;
      }
      j++;
    }
    i++;
  }
  buddy->max=0;
}

static int bin_index(int size,int bin_size){
  if(size==0)
    return -1;
  assert(bin_size>=size);
  int i=DEFAULT_LEVEL-1;
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

static void* get_available(void **bin,int size,int bin_size){
  int i=bin_index(size,bin_size);
  buddy_t *buddy;
  int j=i;
  while(j<DEFAULT_LEVEL-1){
    buddy=(buddy_t*)*(bin+j);
    if(buddy!=NULL)
      break;
    j++;
  }
  if(buddy==NULL){
    buddy=init_buddy(bin_size);
    buddy->bin=bin;
  }
  int max=buddy->max;
  void* result=get_buddy_mem(buddy,DEFAULT_LEVEL-i-1);
  if(max!=buddy->max){
    adjust_bin(buddy,i);
  }
  return result;
}

static void adjust_bin(buddy_t *buddy,int origin_index){
  if(origin_index!=-1){ 
    *(buddy->bin+origin_index)=buddy->next;
  }
  if(buddy->max==0){
    return;
  }
  int index=bin_index(buddy->max,buddy->size);
  buddy_t* temp=*(buddy->bin+index);
  *(buddy->bin+index)=buddy;
  buddy->next=temp;
}

static void* direct_alloc(int size){
  mem_chunk_t *chunk=(mem_chunk_t *)malloc(size);
  return USER_MEM(chunk);
}

void free_mem(void *mem){
  mem_chunk_t *chunk=CHUNK_MEM(mem);
  if(chunk->is_direct){
    free(chunk);
  }else{
    free_chunk(chunk);
  }
}

static void free_chunk(mem_chunk_t *chunk){
  buddy_t *buddy=chunk->buddy;
  int max=buddy->max;
  int origin_index=bin_index(buddy->max,buddy->size);
  int diff=((void*)chunk - buddy->base)/(buddy->size>>chunk->level);
  int index=pow(2,chunk->level)-1+diff;
  update_buddy_flag_unuse(buddy,index,1);  
  update_buddy_max(buddy);
  if(max!=buddy->max){
    adjust_bin(buddy,origin_index);
  }
}
