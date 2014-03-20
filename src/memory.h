#include "list.h"
#define DEFAULT_LEVEL 7
#define DEFAULT_FLAG_NUM (2<<DEFAULT_LEVEL)-1
#define SMALL_THRESHOLD 1<<10
#define BIG_THRESHOLD 1<<20

/**
 * mem_buddy_chunk_t 后面的空间全都是可用的，返回给调用者的
 */
#define USER_BUDDY_MEM(chunk) (void*)(chunk)+sizeof(mem_buddy_chunk_t)

/**
 * mem_direct_chunk_t 后面的空间全都是可用的，返回给调用者的
 */
#define USER_DIRECT_MEM(chunk) (void*)(chunk)+sizeof(mem_direct_chunk_t)

/**
 * 反向找到mem_buddy_chunk_t的首地址
 */
#define CHUNK_BUDDY_MEM(mem) (mem_buddy_chunk_t *)(mem-sizeof(mem_buddy_chunk_t))

/**
 * 反向找到mem_direct_chunk_t的首地址
 */
#define CHUNK_DIRECT_MEM(mem) (mem_direct_chunk_t *)(mem-sizeof(mem_direct_chunk_t))

#define FLAG_MEM(mem) (int *)(mem-sizeof(int))


#define LEFT_CHILD(index) (2*index+1)

#define RIGHT_CHILD(index) (2*index+2)

#define PARENT(index) (index-1)>>1

#define LEVEL_START(level) (pow(2,level)-1)

#define LEVEL_END(level) (pow(2,level+1)-2)

struct buddy_s{
  /**
   * 具有相同max的buddy的双向链表
   */
  list_head_t list;
  /**
   * buddy 的标志位
   */
  int flags[DEFAULT_FLAG_NUM];

  /**
   * 当时buddy能分配的最大空间
   */
  int max;

  /**
   * buddy初始空间的大小
   */
  int size;

  /**
   * buddy 分配空间的启示地址
   */
  void *base; 

  /**
   * buddy 所属于的bin
   */ 
  void **bin;
};

typedef struct buddy_s buddy_t;

/**
 * 代表一段由buddy分配出去的内存，chunk->is_direct后面的数据就是user mem
 */
struct mem_buddy_chunk_s{
   
  /**
   * 标志这段空间属于哪个buddy,释放的时候以便更新buddy的相关flag
   */
   buddy_t *buddy;
  /**
   * 方便及时找到buddy的flag的层级，然后根据地址offset，来确定具体的flag index
   */
   int level;
   /**
   * 标志这段空间是否是malloc直接分配，如果是，释放的时候直接调用free函数
   */
   int is_direct;

};
typedef struct mem_buddy_chunk_s mem_buddy_chunk_t;

/**
 * 代表一段由malloc分配的内存，chunk->is_direct后面的数据就是user mem
 */
struct mem_direct_chunk_s{
  /**
   * 直接malloc分配的内存，建立双链表的目的是为了free的时候，快速移除链表。这链表为了再destory_pool的时候释放空间
   */
  list_head_t list;
   
  int is_direct;
};

typedef struct mem_direct_chunk_s mem_direct_chunk_t;

/**
 * mem pool有两种 buddy容器，当请求小于SMALL_THRESHOLD的时候，请求small_bin容器，然后分配buddy
 * 如果请求小于BIG_THRESHOLD的时候，请求big_bin容器，对于大于BIG_THRESHOLD的请求，直接malloc分配
 */
struct mem_pool_s{
  void** small_bin;
  void** big_bin;
  list_head_t* direct_head;
};

typedef struct mem_pool_s mem_pool_t;

mem_pool_t* init_mem_pool();

void* alloc_mem(mem_pool_t *pool,int size);

void free_mem(void *mem);

void destory_mem_pool(mem_pool_t *pool);
