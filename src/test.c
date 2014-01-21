#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
  hash_t *hash=init_hash();
  put("a","a",hash);
  put("e","e",hash);
  puts(get("a",hash));
}

