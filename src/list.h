#ifndef LIST_H
#define LIST_H

struct list_head_s{
  struct list_head_s *next,*prev;
};

typedef struct list_head_s list_head_t;


void init_list(list_head_t *head);


void list_add_data(list_head_t *n,list_head_t *next,list_head_t *prev);


void list_del_data(list_head_t *prev,list_head_t *next);


int list_is_empty(list_head_t *head);
#endif
