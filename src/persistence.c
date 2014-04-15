#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include "queue.h"
#include "list.h"
#include "hash.h"
#include "my_log.h"
#include "item.h"
#include "memory.h"
#include "buffer.h"
#include "dy_char.h"

#define QUEUE_DATA_DIR "/queue/"

#define FILE_SUFFIX ".dat"

#define SYNC_INTERVAL 1

#define FILE_MAX 1024*1024

#define WRITE_BUFFER_SIZE 4*1024

#define BUFFER_SIZE 1024*4

static void *data_sync_loop(void *arg);

static int open_queue_file(char *key);

static void sync_set();

static void sync_get();

static void start_recovery();

static DIR* open_queue_dir();

static void recovery_queue(struct dirent *p_dirent);

static void store_data(char_t *dy_char,char *key);

static hash_t *sync_set_hash;

static hash_t *sync_get_hash;

static hash_t *sync_get_offset_hash;

static const char *avialble="1";

static const char *unavialble="0";

static const char *current_dir=".";

static const char *parent_dir="..";

static const char special_r='\r';

static const char special_n='\n';

static buffer_t *buf;

enum RECOVERY_STATUS{
	START,
	STORE,
	DISCARD,
	DISCARD_END,
	STORE_END
};

extern mem_pool_t *pool;

extern hash_t *hash;

void init_sync(){
	sync_set_hash=init_hash();
	sync_get_hash=init_hash();
	sync_get_offset_hash=init_hash();
	buf=alloc_buffer(BUFFER_SIZE);
	start_recovery();
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
			while(1){
				int *i=pop(q);
				if(i==NULL){
					break;
				}
				lseek(fd,*o,SEEK_SET);		
				int result=write(fd,unavialble,1);			
				(*o)+=3;
				(*o)+=*i;
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
	dir[index]='\0';
	int fd=open(dir,O_RDWR|O_CREAT);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
	}
	return fd;
}

static DIR* open_queue_dir(){
	int index=0;
	char *log_dir=QUEUE_DATA_DIR;
	int length=strlen(DAWN_HOME);
	int log_length=strlen(log_dir);
	char dir[length+log_length+1];
	memcpy(dir,DAWN_HOME,length);
	index+=length;
	memcpy(dir+index,log_dir,log_length);
	index+=log_length;
	dir[index]='\0';
	DIR *p_dir;
	if((p_dir=opendir(dir))==NULL){
		my_log(ERROR,"open dir error\n");
		return NULL;
	}
	return p_dir;
}

static void start_recovery(){
	DIR *dir=open_queue_dir();
	struct dirent *p_dirent;
	while((p_dirent=readdir(dir))){
		if(strcmp(p_dirent->d_name,current_dir)==0){
			continue;
		}else if(strcmp(p_dirent->d_name,parent_dir)==0){
			continue;
		}else{
			recovery_queue(p_dirent);
		}
	}
	closedir(dir);
}

static void recovery_queue(struct dirent *p_dirent){
	char *key=strtok(p_dirent->d_name,".");
	int fd=open_queue_file(key);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
		return ;
	}
	enum RECOVERY_STATUS status=START;
	char_t *dy_char=init_char();
	while(1){
		int count=read(fd,buf->data+buf->limit,buf->size-buf->limit);
		if(count<=0){
			break;
		}
		buf->limit+=count;
		while(has_remaining(buf)){
			char c=*(buf->data+buf->current);
			if(status==START){
				if(c=='1'){
					status=STORE;
				}else{
					status=DISCARD;
				}
			}else if(status==STORE){
				add_char(dy_char,c);
				if(c==special_r){
					status=STORE_END;
				}
			}else if(status==STORE_END){
				add_char(dy_char,c);
				if(c==special_n){
					store_data(dy_char,key);
					reset_char(dy_char);
					status=START;
				}
			}else if(status==DISCARD&&c==special_r){
				status=DISCARD_END;
			}else if(status==DISCARD_END&&c==special_n){
				status=START;
			} 
			buf->current+=1;
		}
		compact(buf);
	}
	close(fd);
}

static void store_data(char_t *dy_char,char *key){
	int size=dy_char->current;
	item_t *i=init_item();
	char *c=(char *)alloc_mem(pool,size);
	memcpy(c,dy_char->data,size);
	i->data=c;
	i->end=1;
	i->c_size=size;
	i->size=size-2;
	queue_t *q=(queue_t *)get(key,hash);
	if(q==NULL){
		q=init_queue();
		char *q_name=(char *)alloc_mem(pool,strlen(key));
		strcpy(q_name,key);
		put(q_name,q,hash);
	}
	push(q,i);
}
