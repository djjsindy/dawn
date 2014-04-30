#include <stdio.h>
#include <string.h>
#include "statistics.h"
#include "memory.h"
#include "hash.h"

stat_t *stat;

extern mem_pool_t *pool;

void init_stat(){
	stat=(stat_t *)malloc(sizeof(stat_t));
	int size=sizeof(unsigned long)*(DEFAULT_LEVEL+1);
	stat->small_buddy_stats=calloc(DEFAULT_LEVEL+1,sizeof(unsigned long));
	stat->big_buddy_stats=calloc(DEFAULT_LEVEL+1,sizeof(unsigned long));
}

void init_queue_stat(){
	stat->queue_data_hash=init_hash();
}

void start_queue_stat(char *key){
	queue_stat_t *qs=(queue_stat_t *)alloc_mem(pool,sizeof(queue_stat_t));
	put(key,qs,stat->queue_data_hash);
	qs->name=key;
	qs->size=0;
	qs->bytes=0;
}

void increase_queue_stat(char *key,int delta_bytes){
	queue_stat_t *qs=get(key,stat->queue_data_hash);
	qs->size++;
	qs->bytes+=delta_bytes;
}

void decrease_queue_stat(char *key,int delta_bytes){
	queue_stat_t *qs=get(key,stat->queue_data_hash);
	qs->size--;
	qs->bytes-=delta_bytes;
}

