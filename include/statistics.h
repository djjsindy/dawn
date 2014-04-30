#include "hash.h"
#include "memory.h"
struct queue_stat_s{
	char *name;
	unsigned long size;
	unsigned long bytes;
};

typedef struct queue_stat_s queue_stat_t;

struct stat_s{
	unsigned long total_items;
	int current_connections;
	unsigned long *small_buddy_stats;
	unsigned long *big_buddy_stats;
	unsigned long direct_size;
	hash_t *queue_data_hash;
};

typedef struct stat_s stat_t;

void init_stat();

void init_queue_stat();

void start_queue_stat(char *key);

void increase_queue_stat(char *key,int delta_bytes);

void decrease_queue_stat(char *key,int delta_bytes);

