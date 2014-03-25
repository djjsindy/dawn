#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memory.h"

int main(){
  mem_pool_t *pool=init_mem_pool();
  //void *p=alloc_mem(pool,8);
  void *q=alloc_mem(pool,24);
}
