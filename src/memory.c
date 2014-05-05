#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include "memory.h"
#include "my_log.h"
#include "statistics.h"

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

static void* get_available(list_head_t **bin,intptr_t size,intptr_t bin_size,mem_pool_t *pool);

static intptr_t bin_index(intptr_t size,intptr_t bin_size);

static buddy_t* init_buddy(intptr_t size,mem_pool_t *pool);

static mem_buddy_chunk_t* get_buddy_mem(buddy_t *buddy,intptr_t level);

static void update_buddy_max(buddy_t *buddy);

static intptr_t adjust_bin(buddy_t *buddy);

static void* direct_alloc(list_head_t *head,intptr_t size,mem_pool_t *pool);

static void free_buddy_chunk(mem_buddy_chunk_t *chunk);

static void free_direct_chunk(mem_direct_chunk_t *chunk);

static void update_child_flag(buddy_t *buddy,intptr_t index,intptr_t flag);

static void update_buddy_flag_inuse(buddy_t *buddy,intptr_t index);

static void update_buddy_flag_unuse(buddy_t *buddy,intptr_t index);

static void free_buddy_bin(list_head_t **bin);

static void free_direct_list(list_head_t *head);

static intptr_t head_size();

static void init_pool_list(list_head_t **base,intptr_t num);

static void init_pthread_mutex(mem_pool_t *pool);

static void * realloc_direct_chunk(mem_direct_chunk_t *old_chunk,intptr_t new_size);

extern stat_t *stat;


/**
 *  初始化内存池内存结构，初始化small bin，big bin，和direct队列,初始化每个bin中的第一个元素（list_head_t）,初始化每个bin的锁
 */
mem_pool_t* init_mem_pool(){
  intptr_t num=DEFAULT_LEVEL+1;
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

/**
 * 初始化为bin每一个位置链表初始化一个head
 */
static void init_pool_list(list_head_t **base,intptr_t num){
  intptr_t index=0;
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

/**
 *  销毁一个mem_pool
 */
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

/**
 * 释放direct 所占有的内存
 */
static void free_direct_list(list_head_t *head){
  list_head_t *next=head->next;
  while(next!=head){
    mem_direct_chunk_t *chunk=(mem_direct_chunk_t *)next;
    next=chunk->list.next;
    free(chunk);
  }
}

/**
 * 释放bin中所有buddy占有的内存
 */
static void free_buddy_bin(list_head_t **bin){
  intptr_t index=0;
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

/**
 * 根据request size来决定是通过small bin还是big bin还是direct 来分配内存
 */
void* alloc_mem(mem_pool_t *pool,intptr_t size){
  intptr_t real_size=size+head_size();
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

/**
 * 每段内存之前是所对应结构体的数据结构，需要分配一个统一大小的头，那么取最大值
 */
static intptr_t head_size(){
  intptr_t buddy_s_size=sizeof(mem_buddy_chunk_t);
  intptr_t direct_s_size=sizeof(mem_direct_chunk_t);
  return buddy_s_size>direct_s_size?buddy_s_size:direct_s_size;
}

/**
 * 初始化一个buddy
 */
static buddy_t* init_buddy(intptr_t size,mem_pool_t *pool){

  buddy_t *buddy=(buddy_t *)malloc(sizeof(buddy_t));

  if(buddy==NULL){
    my_log(ERROR,"alloc new buddy failed\n");
    return NULL;
  }

  intptr_t i=0;
  //flag置为1，表示可用
  for(;i<sizeof(buddy->flags)/sizeof(intptr_t);i++){
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

/**
 * 确定了buddy，那么肯定可以从这个buddy分配内存，level已经确定，那么直接更新所对应的
 * flag，然后返回内存起始地址
 */
static mem_buddy_chunk_t* get_buddy_mem(buddy_t *buddy,intptr_t level){
  intptr_t size=buddy->size;
  intptr_t start=LEVEL_START(level);
  intptr_t end=LEVEL_END(level);
  intptr_t index=start;
  mem_buddy_chunk_t *chunk;

  //寻找这个level可用的位置，肯定能找到
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

  //由于分配出去了内存，那么更新相关位置的flag为0
  update_buddy_flag_inuse(buddy,index);

  //重新max的值，
  update_buddy_max(buddy);
  return chunk;
}

/**
 * 更新buddy的index位置所对应的内存为占用状态，先更新父节点的状态
 */
static void update_buddy_flag_unuse(buddy_t *buddy,intptr_t index){
  intptr_t parent;
  intptr_t source=index;

  //递归更新父节点的状态
  while(index>0&&(parent=PARENT(index))>=0){

    //如果父节点是可用状态,那么表示上层结构不用更新了
    if(buddy->flags[parent]==1){
      break;
    }else{

      //如果是不可用状态，那么需要区分这个不可用状态，是由当前节点导致的还是由兄弟节点导致的
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

  //递归更新子节点的状态
  update_child_flag(buddy,source,1);
}

/**
 * 更新flag为占用状态
 */
static void update_buddy_flag_inuse(buddy_t *buddy,intptr_t index){

  /**
   * 更新parent flag
   */
  intptr_t parent=PARENT(index);
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

/**
 * 递归更新子节点状态
 */
static void update_child_flag(buddy_t *buddy,intptr_t index,intptr_t flag){  
  if(index>sizeof(buddy->flags)/sizeof(intptr_t)-1){
    return;
  }
  buddy->flags[index]=flag;
  update_child_flag(buddy,LEFT_CHILD(index),flag);
  update_child_flag(buddy,RIGHT_CHILD(index),flag);
}

/**
 * 更新buddy max,过程从flag二叉树顶层开始，依次下降，如果碰到这层全部为0，那么
 * 层数下降一层，如果这层有不为0的，那么max就是这层所对应的max值，如果递归到了最
 * 后一层，那么max就是0
 */
static void update_buddy_max(buddy_t *buddy){
  intptr_t level=0;
  while(level<DEFAULT_LEVEL){
    intptr_t start=LEVEL_START(level);
    intptr_t end=LEVEL_END(level);
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

/**
 * 确定bin_size的第几层能满足分配size的需求
 */
static intptr_t bin_index(intptr_t size,intptr_t bin_size){
  if(size==0)
    return 0;
  assert(bin_size>=size);
  intptr_t i=DEFAULT_LEVEL;
  while(i>0){
    if(size==bin_size){
      return i;
    }else if(size>bin_size){
      return i+1;
    }
    bin_size/=2;
    i--;
  }
  return 1;
}

/**
 * 从bin中获得mem
 */
static void* get_available(list_head_t **bin,intptr_t size,intptr_t bin_size,mem_pool_t *pool){
  buddy_t *buddy=NULL;
  intptr_t *buddy_stat;

  //初始化统计
  if(bin==pool->small_bin){
    buddy_stat=stat->small_buddy_stats;
  }else{
    buddy_stat=stat->big_buddy_stats;
  }
  intptr_t level=bin_index(size,bin_size);

  //找合适的buddy
  intptr_t test=level;
  while(test<=DEFAULT_LEVEL){
    list_head_t *head=(list_head_t *)*(bin+test);
    if(!list_is_empty(head)){
      buddy=list_entry(head->next,buddy_t,list);
      level=test;
      break;
    }
    test++;
  }

  intptr_t create=0;
  //如果没有找到，就创建一个，暂时不入双链表
  if(buddy==NULL){  
    buddy=init_buddy(bin_size,pool);
    buddy->bin=bin;
    create=1;
  }

  //从这个buddy中获得mem
  intptr_t max=buddy->max;
  mem_buddy_chunk_t *chunk=get_buddy_mem(buddy,DEFAULT_LEVEL-level);
  chunk->size=size;

  //如果max有变化，更新buddy的位置，更新统计
  if(max!=buddy->max){

    //如果不是新创建的，那么就调整buddy的位置
    if(!create){
      intptr_t new_level=adjust_bin(buddy);
      *(buddy_stat+level)-=1;
      *(buddy_stat+new_level)+=1;
    }else{
      //插入双链表
      intptr_t level=bin_index(buddy->max,buddy->size);
      list_head_t *head=*(bin+level);
      list_add_data(&buddy->list,head,head->next);
      *(buddy_stat+level)+=1;
    }
  }
  return USER_BUDDY_MEM(chunk);
}

/**
 * 如果max和获得mem后的buddy max不一致，那么就要调整buddy的位置，调整到应该的位置
 */
static intptr_t adjust_bin(buddy_t *buddy){

  //从当前双链表中删除
  list_del_data(buddy->list.prev,buddy->list.next);

  //添加到新的双链表中
  intptr_t new_index=bin_index(buddy->max,buddy->size);
  list_head_t *head=*(buddy->bin+new_index);
  list_add_data(&buddy->list,head,head->next);
  return new_index;
}

/**
 * 直接调用malloc分配内存
 */
static void* direct_alloc(list_head_t *list, intptr_t size,mem_pool_t *pool){
  mem_direct_chunk_t *chunk=(mem_direct_chunk_t *)malloc(size);
  chunk->is_direct=1;
  chunk->pool=pool;
  list_add_data(&chunk->list,list,list->next);
  stat->direct_size++;
  return USER_DIRECT_MEM(chunk);
}

/**
 * 释放空间，mem首地址之前的int表示是否是buddy分配还是direct分配
 */
void free_mem(void *mem){
  intptr_t flag=*(FLAG_MEM(mem));
  if(flag==1){
    mem_direct_chunk_t *chunk=CHUNK_DIRECT_MEM(mem);
    free_direct_chunk(chunk);
  }else{
    mem_buddy_chunk_t *chunk=CHUNK_BUDDY_MEM(mem);
    free_buddy_chunk(chunk);
  }
}

/**
 * 回收direct分配内存，直接调用free
 */
static void free_direct_chunk(mem_direct_chunk_t *chunk){
  pthread_mutex_lock(&(chunk->pool->direct_mutex));
  list_del_data(chunk->list.prev,chunk->list.next); 
  free(chunk);
  stat->direct_size--;
  pthread_mutex_unlock(&(chunk->pool->direct_mutex));
}

/**
 * 释放buddy内存
 */
static void free_buddy_chunk(mem_buddy_chunk_t *chunk){
  intptr_t *buddy_stat;
  buddy_t *buddy=chunk->buddy;
  pthread_mutex_t *mutex;

  //获得锁
  if(buddy->size==SMALL_THRESHOLD){
    mutex=&(chunk->pool->small_mutex);
    buddy_stat=stat->small_buddy_stats;
  }else{
    mutex=&(chunk->pool->big_mutex);
    buddy_stat=stat->big_buddy_stats;
  }
  pthread_mutex_lock(mutex);
  intptr_t max=buddy->max;
  intptr_t level=bin_index(buddy->max,buddy->size);
  //计算level下，分配的内存是属于哪个flag控制
  intptr_t diff=((void*)chunk - buddy->base)/(buddy->size>>chunk->level);
  intptr_t index=LEVEL_START(chunk->level)+diff;

  //当然更新flag和max
  update_buddy_flag_unuse(buddy,index);  
  update_buddy_max(buddy);

  //更新buddy的位置
  if(max!=buddy->max){
    intptr_t new_level=adjust_bin(buddy);
    *(buddy_stat+level)-=1;
    *(buddy_stat+new_level)+=1;
  }
  pthread_mutex_unlock(mutex);
}

