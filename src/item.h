#ifndef ITEM_H
#define ITEM_H
struct item_s{
  char *data;
  int end;
  int size;
};

typedef struct item_s item_t;

item_t *init_item();

void destroy_item(item_t *i);
#endif
