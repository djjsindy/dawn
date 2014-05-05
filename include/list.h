#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
struct list_head_s{
  struct list_head_s *next,*prev;
};

typedef struct list_head_s list_head_t;

struct list_entry_s{
  void* p;
  list_head_t list;
};

typedef struct list_entry_s list_entry_t;

void init_list(list_head_t *head);


void list_add_data(list_head_t *n,list_head_t *prev,list_head_t *next);


list_head_t* list_del_data(list_head_t *prev,list_head_t *next);


intptr_t list_is_empty(list_head_t *head);


#define offsetof(type, member)  (size_t)(&((type*)0)->member)
 
#define container_of(ptr, type, member) ({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \
    (type *)((char *)__mptr - offsetof(type, member)); })

#define list_entry(ptr, type, member) \
      container_of(ptr, type, member)
#endif
