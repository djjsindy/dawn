#ifndef ITEM_H
#define ITEM_H
#include "list.h"

struct item_s{
  char *data;
  int end;
  int size;
  list_head_t list;
};

typedef struct item_s item_t;

struct write_data_s{
  char *data;
  list_head_t list;
};

typedef struct write_data_s write_data_t;

item_t *init_item();

void destroy_item(item_t *i);
#endif
