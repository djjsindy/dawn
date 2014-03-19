struct list_head_s{
  struct list_head_s *next,*prev;
}

typedef struct list_head_s list_head_t;

#define INIT_LIST(list_head) do{
   list_head->next=list_head;
   list_head->prev=list_head;
}while(0) 

#define LIST_ADD_DATA(new,prev,next) do{
  new->next=next;
  next->prev=new;
  new->prev=prev;
  prev->next=new;
}while(0)

#define LIST_DEL_DATA(prev,next) do{
  prev->next=next;
  next->prev=prev;
}while(0)
