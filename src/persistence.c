#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "queue.h"
#include "list.h"
#include "hash.h"
#include "my_log.h"
#include "item.h"
#include "memory.h"

#define QUEUE_DATA_DIR "/queue/"

#define FILE_SUFFIX ".dat"

#define SYNC_INTERVAL 1

#define FILE_MAX 1024*1024

#define WRITE_BUFFER_SIZE 4*1024

static void *data_sync_loop(void *arg);

static int open_queue_file(char *key);

static void sync_set();

static void sync_get();

static hash_t *sync_set_hash;

static hash_t *sync_get_hash;

static hash_t *sync_get_offset_hash;

static const char *avialble="1";

static const char *unavialble="0";

extern mem_pool_t *pool;

void init_sync(){
	sync_set_hash=init_hash();
	sync_get_hash=init_hash();
	sync_get_offset_hash=init_hash();
	pthread_t tid=0;
	pthread_create(&tid,NULL,data_sync_loop,NULL);
}

static void *data_sync_loop(void *arg){
	while(1){
		sync_set();
		sync_get();
		sleep(SYNC_INTERVAL);
	}
}

static void sync_set(){
	list_head_t *head=visit_hash(sync_set_hash);
	while(!list_is_empty(head)){
		hash_entry_t *entry=(hash_entry_t *)list_entry(head->next,hash_entry_t,list);
		queue_t *q=(queue_t *)entry->data;
		if(queue_size(q)>0){
			int fd=open_queue_file(entry->key);
			lseek(fd,0,SEEK_END);
			int first=1;
			while(1){
				item_t *i=pop(q);
				if(i==NULL){
					break;
				}
				if(first){
					write(fd,avialble,1);
					first=0;
				}
				write(fd,i->data,i->c_size);
				if(i->end==1){
					first=1;
				}
			}
			close(fd);		
		}
		free_mem(entry);
		list_del_data(head,head->next->next);	
	}
	free_mem(head);
}

static void sync_get(){
	list_head_t *head=visit_hash(sync_get_hash);
	while(!list_is_empty(head)){
		hash_entry_t *entry=(hash_entry_t *)list_entry(head->next,hash_entry_t,list);
		queue_t *q=(queue_t *)entry->data;
		if(queue_size(q)>0){
			int fd=open_queue_file(entry->key);
			off_t *o=(off_t *)get(entry->key,sync_get_offset_hash);
			if(o==NULL){
				o=alloc_mem(pool,sizeof(off_t));
				*o=0;
				put(entry->key,o,sync_get_offset_hash);
			}
			off_t offset=*o;
			while(1){
				int *i=pop(q);
				if(i==NULL){
					break;
				}
				lseek(fd,offset,SEEK_SET);		
				int result=write(fd,unavialble,1);			
				offset+=1;
				offset+=*i;
				free_mem(i);
			}
			close(fd);
		}
		list_del_data(head,head->next->next);
		free_mem(entry);
	}
	free_mem(head);
}

void add_set_sync_data(char *key,item_t *i){
	queue_t *q=get(key,sync_set_hash);
	if(q==NULL){
		int k_length=strlen(key);
		char *k=(char *)alloc_mem(pool,k_length+1);
		memcpy(k,key,k_length);
		*(k+k_length)='\0';
		q=init_queue();
		put(k,q,sync_set_hash);
	}
	push(q,i);
}

void add_get_sync_data(char *key,int size){
	queue_t *q=get(key,sync_get_hash);
	if(q==NULL){
		int k_length=strlen(key);
		char *k=(char *)alloc_mem(pool,k_length+1);
		memcpy(k,key,k_length);
		*(k+k_length)='\0';
		q=init_queue();
		put(k,q,sync_get_hash);
	}
	int *i=(int *)alloc_mem(pool,sizeof(int));
	*i=size;
	push(q,i);
}

static int open_queue_file(char *key){
	char *log_dir=QUEUE_DATA_DIR;
	int length=strlen(DAWN_HOME);
	int log_length=strlen(log_dir);
	int name_length=strlen(key);
	int suffix_length=strlen(FILE_SUFFIX);
	char dir[length+log_length+name_length+suffix_length+1];
	int index=0;
	memcpy(dir,DAWN_HOME,length);
	index+=length;
	memcpy(dir+index,log_dir,log_length);
	index+=log_length;
	memcpy(dir+index,key,name_length);
	index+=name_length;
	memcpy(dir+index,FILE_SUFFIX,suffix_length);
	index+=suffix_length;
	dir[length+index]='\0';
	int fd=open(dir,O_RDWR|O_CREAT);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
	}
	return fd;
}

