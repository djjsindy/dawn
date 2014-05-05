#ifndef ITEM_H
#define ITEM_H
#include <stdint.h>

struct item_s{
  char *data;
  intptr_t end;
  intptr_t size;
  intptr_t c_size;
};

typedef struct item_s item_t;

item_t *init_item();

void destroy_item(item_t *i);
#endif
