#include <stdint.h>
#include "hash.h"
#include "memory.h"
struct queue_stat_s{
	char *name;
	intptr_t size;
	intptr_t bytes;
};

typedef struct queue_stat_s queue_stat_t;

struct stat_s{
	intptr_t total_items;
	intptr_t current_connections;
	intptr_t *small_buddy_stats;
	intptr_t *big_buddy_stats;
	intptr_t direct_size;
	hash_t *queue_data_hash;
};

typedef struct stat_s stat_t;

void init_stat();

void init_queue_stat();

void start_queue_stat(char *key);

void increase_queue_stat(char *key,intptr_t delta_bytes);

void decrease_queue_stat(char *key,intptr_t delta_bytes);

