#include "list.h"

void init_list(list_head_t *head){
  head->prev=head;
  head->next=head;
}

void list_add_data(list_head_t *n,list_head_t *prev,list_head_t *next){
  n->next=next;
  next->prev=n;           
  n->prev=prev;           
  prev->next=n;           
}


list_head_t* list_del_data(list_head_t *prev,list_head_t *next){
  list_head_t *result=prev->next;
  prev->next=next;          
  next->prev=prev;
  return result;
}

int list_is_empty(list_head_t *head){
  return head->next==head;
}

void list_add_pointer(void *p,list_head_t *prev,list_head_t *next){
  list_entry_t *entry=(list_entry_t *)malloc(sizeof(list_entry_t));
  entry->p=p;
  list_add_data(&(entry->list),prev,next);
}

void* list_del_pointer(list_head_t *prev,list_head_t *next){
  list_head_t *del=list_del_data(prev,next);
  list_entry_t *entry=list_entry(del,list_entry_t,list);
  void *p=entry->p;
  free(entry);
  return p;
}
