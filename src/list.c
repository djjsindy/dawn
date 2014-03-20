#include "list.h"

void init_list(list_head_t *head){
  head->prev=head;
  head->next=head;
}

void list_add_data(list_head_t *n,list_head_t *next,list_head_t *prev){
  n->next=next;
  next->prev=n;           
  n->prev=prev;           
  prev->next=n;           
}


void list_del_data(list_head_t *prev,list_head_t *next){
  prev->next=next;          
  next->prev=prev;          
}

int list_is_empty(list_head_t *head){
  return head->next==head;
}
