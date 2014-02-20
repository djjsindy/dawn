#define DEFAULT_LEVEL 7
#define DEFAULT_FLAG_NUM (2<<DEFAULT_LEVEL)-1
#define SMALL_THRESHOLD 1<<10
#define BIG_THRESHOLD 1<<20

#define USER_MEM(chunk) (void*)(chunk)+sizeof(mem_chunk_t)

#define CHUNK_MEM(mem) (mem_chunk_t *)(mem-sizeof(mem_chunk_t))

struct buddy_s{
  int flags[DEFAULT_FLAG_NUM];
  int max;
  int size;
  struct buddy_s *next;
  void *base;
  void **bin;
};

typedef struct buddy_s buddy_t;

struct mem_chunk_s{
   buddy_t *buddy;
   int is_direct;
   int level;
};
typedef struct mem_chunk_s mem_chunk_t;

struct mem_pool_s{
  void** small_bin;
  void** big_bin;
};

typedef struct mem_pool_s mem_pool_t;

mem_pool_t* init_mem_pool();

void* alloc_mem(mem_pool_t *pool,int size);

void free_mem(void *mem);
