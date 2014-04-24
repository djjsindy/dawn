#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <setjmp.h>
#include "memory.h"
#include "my_log.h"
/**                  max(b)  0  16  32  64 128 256 512  1024
 *  mem_pool---- small_bin---0---1---2---3---4---5---6---7
 *           |                        |               |
 *           |                    list_head_t     list_head_t
 *           |                        | 
 *           |                    list_head_t
 *           |
 *           |   
 *           |      max(kb) 0   16  32  64 128 256 512  1024
 *           |----big_bin---0---1---2---3---4---5---6---7
 *           |              |               |           |
 *           |          list_head_t   list_head_t   list_head_t
 *           |                              |           
 *           |                        list_head_t
 *           |
 *           |
 *           |
 *           |>1Mb
 *           |---list_head_t->list_head_t->list_head_t->list_head_t
 *  
 *
 * mem_pool由small_bin,big_bin和direct list组成，small_bin接收1-1kb的请求，big_bin
 * 接收1kb－1Mb的请求，其他大于1Mb的请求，直接调用malloc，每个bin是一个list_head_t的数组，
 * 默认8个位置，每个list_head_t代表一个buddy，每个位置是一个buddy的双链表，每次分配根据请
 * 求确定了bin和位置，如果当前index没有buddy，继续看下一个位置，如果都失败了，分配新的buddy，
 * 从buddy中请求完空间后，更新buddy的max，如果max有变化，就调整buddy的index，在index之间
 * 根据max调整位置,位置为0的index，记录的是buddy没有空间分配的链表，记录这个链表是为了在释
 * 放空间后，再次调整buddy的位置。
 * 
 *
 */

static void* get_available(list_head_t **bin,int size,int bin_size,mem_pool_t *pool);

static int bin_index(int size,int bin_size);

static buddy_t* init_buddy(int size,mem_pool_t *pool);

static mem_buddy_chunk_t* get_buddy_mem(buddy_t *buddy,int level);

static void update_buddy_max(buddy_t *buddy);

static void adjust_bin(buddy_t *buddy);

static void* direct_alloc(list_head_t *head,int size,mem_pool_t *pool);

static void free_buddy_chunk(mem_buddy_chunk_t *chunk);

static void free_direct_chunk(mem_direct_chunk_t *chunk);

static void update_child_flag(buddy_t *buddy,int index,int flag);

static void update_buddy_flag_inuse(buddy_t *buddy,int index);

static void update_buddy_flag_unuse(buddy_t *buddy,int index);

static void free_buddy_bin(list_head_t **bin);

static void free_direct_list(list_head_t *head);

static int head_size();

static void init_pool_list(list_head_t **base,int num);

static void init_pthread_mutex(mem_pool_t *pool);

static void * realloc_direct_chunk(mem_direct_chunk_t *old_chunk,int new_size);


/**
 *  初始化内存池内存结构，初始化small bin，big bin，和direct队列,初始化每个bin中的第一个元素（list_head_t）,初始化每个bin的锁
 */
mem_pool_t* init_mem_pool(){
  int num=DEFAULT_LEVEL+1;
  mem_pool_t* pool=(mem_pool_t *)malloc(sizeof(mem_pool_t));
  assert(pool!=NULL);
  pool->small_bin=(list_head_t **)malloc(sizeof(list_head_t *)*(num));  
  pool->big_bin=(list_head_t **)malloc(sizeof(list_head_t *)*(num));
  assert(pool->small_bin);
  assert(pool->big_bin);
  init_pool_list(pool->small_bin,num);
  init_pool_list(pool->big_bin,num);
  init_list(&(pool->direct_head));
  init_pthread_mutex(pool);
  return pool;
}


/**
 *  初始化内存池的锁
 */
static void init_pthread_mutex(mem_pool_t *pool){
  pthread_mutex_init(&(pool->small_mutex), NULL);
  pthread_mutex_init(&(pool->big_mutex), NULL);
  pthread_mutex_init(&(pool->direct_mutex), NULL);
}

static void init_pool_list(list_head_t **base,int num){
  int index=0;
  for(;index<num;){
    list_head_t *head=(list_head_t *)malloc(sizeof(list_head_t));
    if(head==NULL){
      my_log(ERROR,"alloc mem pool bin list_head_t error\n");
      return;
    }
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
  int real_size=size+head_size();
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

static int head_size(){
  int buddy_s_size=sizeof(mem_buddy_chunk_t);
  int direct_s_size=sizeof(mem_direct_chunk_t);
  return buddy_s_size>direct_s_size?buddy_s_size:direct_s_size;
}

static buddy_t* init_buddy(int size,mem_pool_t *pool){

  buddy_t *buddy=(buddy_t *)malloc(sizeof(buddy_t));

  if(buddy==NULL){
    my_log(ERROR,"alloc new buddy failed\n");
    return NULL;
  }

  int i=0;
  for(;i<sizeof(buddy->flags)/sizeof(int);i++){
    buddy->flags[i]=1;
  }
  void *mem=malloc(size);
  if(mem==NULL){
    my_log(ERROR,"alloc new buddy base failed\n");
    return NULL;
  }

  buddy->base=mem;
  buddy->size=size;
  buddy->max=size;
  buddy->pool=pool;
  return buddy;
}

static mem_buddy_chunk_t* get_buddy_mem(buddy_t *buddy,int level){
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
  return chunk;
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
  mem_buddy_chunk_t *chunk=get_buddy_mem(buddy,DEFAULT_LEVEL-level);
  chunk->size=size;
  if(max!=buddy->max){
    adjust_bin(buddy);
  }
  return USER_BUDDY_MEM(chunk);
}

static void adjust_bin(buddy_t *buddy){
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
    free_direct_chunk(chunk);
  }else{
    mem_buddy_chunk_t *chunk=CHUNK_BUDDY_MEM(mem);
    free_buddy_chunk(chunk);
  }
}

static void free_direct_chunk(mem_direct_chunk_t *chunk){
  pthread_mutex_lock(&(chunk->pool->direct_mutex));
  list_del_data(chunk->list.prev,chunk->list.next); 
  free(&(chunk->list));
  free(chunk);
  pthread_mutex_unlock(&(chunk->pool->direct_mutex));
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
  int diff=((void*)chunk - buddy->base)/(buddy->size>>chunk->level);
  int index=LEVEL_START(chunk->level)+diff;
  update_buddy_flag_unuse(buddy,index);  
  update_buddy_max(buddy);
  if(max!=buddy->max){
    adjust_bin(buddy);
  }
  pthread_mutex_unlock(mutex);
}

//todo realloc implements 


// void* realloc_mem(void *origin,int new_size){
//   int flag=*(FLAG_MEM(origin));
//   if(flag){
//     mem_direct_chunk_t *chunk=CHUNK_DIRECT_MEM(origin);
//     return realloc_direct_chunk(chunk,new_size);
//   }else{
//     mem_buddy_chunk_t *chunk=CHUNK_BUDDY_MEM(origin);
//     return realloc_buddy_chunk(chunk,new_size);
//   }
// }

// static void * realloc_direct_chunk(mem_direct_chunk_t *old_chunk,int new_size){
//   pthread_mutex_lock(&(old_chunk->pool->direct_mutex));
//   int real_size=new_size+head_size();
//   mem_direct_chunk_t *chunk=(mem_direct_chunk_t *)realloc(old_chunk,real_size);
//   pthread_mutex_unlock(&(chunk->pool->direct_mutex));
//   return USER_DIRECT_MEM(chunk);
// }

// static void * realloc_buddy_chunk(mem_direct_chunk_t *old_chunk,int new_size){
//   pthread_mutex_t *mutex;
//   if(max==SMALL_THRESHOLD){
//     mutex=&(old_chunk->pool->small_mutex);
//   }else{
//     mutex=&(old_chunk->pool->big_mutex);
//   }
//   pthread_mutex_lock(mutex);
//   void *p;
//   buddy_t *buddy=old_chunk->buddy;
//   int size=old_chunk->size;
//   int real_size=new_size+head_size();
//   if(real_size<=size){
//     p=CHUNK_BUDDY_MEM(old_chunk);
//     goto END;
//   }
//   int max=old_chunk->max;
//   int level=old_chunk->level;
//   int level_size=max>>level;
//   int diff=((void*)old_chunk - buddy->base)/(old_chunk->size>>old_chunk->level);
//   //在需要几个level_size
//   int need=((real_size-size)/level_size);
//   if(need==0){
//     p=CHUNK_BUDDY_MEM(old_chunk);
//     goto END;
//   }
//   int index=LEVEL_START(level)+diff;
//   index++;
//   while(index<=LEVEL_END(level)){
//     if(buddy->flags[index]==0){
//       break;
//     }
//     need--;
//     index++;
//     if(need==0){
//       break;
//     }
//   }
  
//   //表示可以根据当前的level，expand 当前chunk到new_size
//   if(need==0){
//     int i=LEVEL_START(level)+diff;
//     while(i<index){
//       buddy->flags[i]=0;
//       update_buddy_flag_inuse(buddy,i);
//     }
//     update_buddy_max(buddy);
//     old_chunk->size=real_size;
//     if(max!=buddy->max){
//       adjust_bin(buddy);
//     }
//     p=CHUNK_BUDDY_MEM(old_chunk);
//   }else{
//     p=alloc_mem(old_chunk->pool,new_size);
//     memcpy(p,USER_BUDDY_MEM(old_chunk),size);
//     update_buddy_flag_unuse(buddy,index);  
//     update_buddy_max(buddy);
//     if(max!=buddy->max){
//       adjust_bin(buddy);
//     }
//   }
// END:
//   pthread_mutex_unlock(mutex);
//   return p;
// }
