#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

void* my_malloc(int size){
  void* result=malloc(size);
  if(result==NULL){
    fprintf(stderr,"alloc memory error\n");
  }
  return result;
}
