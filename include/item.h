#ifndef ITEM_H
#define ITEM_H

struct item_s{
  const char *data;
  int end;
  int size;
  int c_size;
};

typedef struct item_s item_t;

item_t *init_item();

void destroy_item(item_t *i);
#endif
