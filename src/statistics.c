#include <stdio.h>
#include <string.h>
#include "statistics.h"
#include "memory.h"

stat_t *stat;

void init_stat(){
	stat=(stat_t *)malloc(sizeof(stat_t));
	int size=sizeof(unsigned long)*(DEFAULT_LEVEL+1);
	stat->small_buddy_stats=calloc(DEFAULT_LEVEL+1,sizeof(unsigned long));
	stat->big_buddy_stats=calloc(DEFAULT_LEVEL+1,sizeof(unsigned long));
}
