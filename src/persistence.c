#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
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

#define TEMP_FILE_SUFFIX ".dat.tmp"

#define SYNC_INTERVAL 1

#define TRANCATE_FILE_THRESHOLD 1024*1024

#define TRANCATE_RADIO 2

#define BUFFER_SIZE 1024*4

static void *data_sync_loop(void *arg);

static int open_queue_file(char *key);

static int open_queue_temp_file(char *key);

static void sync_set();

static void sync_get();

static void start_recovery();

static DIR* open_queue_dir();

static void recovery_queue(struct dirent *p_dirent);

static void store_data(char_t *dy_char,char *key);

static void remove_queue_file(char *key);

static void *recovery_process(void *arg);

static long queue_file_size(char *key);

static char* queue_file_url(char *key);

static char* queue_temp_file_url(char *key);

static void trancate_queue_file(char *key);

static hash_t *sync_set_hash;

static hash_t *sync_get_hash;

static hash_t *sync_get_offset_hash;

static const char *avialble="1";

static const char *unavialble="0";

static const char *current_dir=".";

static const char *parent_dir="..";

static const char special_r='\r';

static const char special_n='\n';

static buffer_t *rbuf;

static buffer_t *wbuf;

static pthread_mutex_t recovery_lock;

static pthread_cond_t recovery_cond;

static int recovery_count=0;

static int recovery_sum=0;

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
	rbuf=alloc_buffer(BUFFER_SIZE);
	start_recovery();
	pthread_t tid=0;
	pthread_create(&tid,NULL,data_sync_loop,NULL);
}

static void register_recovery_finish(){
  pthread_mutex_lock(&recovery_lock);
  recovery_count++;
  pthread_cond_signal(&recovery_cond);
  pthread_mutex_unlock(&recovery_lock);
} 

static void wait_recovery_finish(){
	while (recovery_count < recovery_sum) {
    	pthread_cond_wait(&recovery_cond, &recovery_lock);
  	}
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
				//todo check result
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
				write(fd,unavialble,1);			
				(*o)+=3;
				(*o)+=*i;
				free_mem(i);
			}
			close(fd);
			int size=queue_file_size(entry->key);
			if(size>=TRANCATE_FILE_THRESHOLD&&(*o)*TRANCATE_RADIO>TRANCATE_FILE_THRESHOLD){
				trancate_queue_file(entry->key);
			}
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
	char *dir=queue_file_url(key);
	int fd=open(dir,O_RDWR|O_CREAT);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
	}
	free_mem(dir);
	return fd;
}

static int open_queue_temp_file(char *key){
	char *dir=queue_temp_file_url(key);
	int fd=open(dir,O_RDWR|O_CREAT);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
	}
	free_mem(dir);
	return fd;
}

static char* queue_file_url(char *key){
	char *log_dir=QUEUE_DATA_DIR;
	int length=strlen(DAWN_HOME);
	int log_length=strlen(log_dir);
	int name_length=strlen(key);
	int suffix_length=strlen(FILE_SUFFIX);
	char *dir=(char *)alloc_mem(pool,length+log_length+name_length+suffix_length+1);
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
	return dir;
}

static char* queue_temp_file_url(char *key){
	char *log_dir=QUEUE_DATA_DIR;
	int length=strlen(DAWN_HOME);
	int log_length=strlen(log_dir);
	int name_length=strlen(key);
	int suffix_length=strlen(TEMP_FILE_SUFFIX);
	char *dir=(char *)alloc_mem(pool,length+log_length+name_length+suffix_length+1);
	int index=0;
	memcpy(dir,DAWN_HOME,length);
	index+=length;
	memcpy(dir+index,log_dir,log_length);
	index+=log_length;
	memcpy(dir+index,key,name_length);
	index+=name_length;
	memcpy(dir+index,TEMP_FILE_SUFFIX,suffix_length);
	index+=suffix_length;
	dir[index]='\0';
	return dir;
}

static long queue_file_size(char *key){
	char *dir=queue_file_url(key);
	unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(dir, &statbuff) < 0){  
        
    }else{  
        filesize = statbuff.st_size;  
    }
    free_mem(dir);  
    return filesize;
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

static void remove_queue_file(char *key){
	char *dir=queue_file_url(key);
	remove(dir);
	free_mem(dir);
}

static void start_recovery(){
	pthread_mutex_init(&recovery_lock, NULL);
    pthread_cond_init(&recovery_cond, NULL);
	DIR *dir=open_queue_dir();
	struct dirent *p_dirent;
	while((p_dirent=readdir(dir))){
		if(strcmp(p_dirent->d_name,current_dir)==0){
			continue;
		}else if(strcmp(p_dirent->d_name,parent_dir)==0){
			continue;
		}else{
			recovery_sum++;
			pthread_t tid=0;
			pthread_create(&tid,NULL,recovery_process,p_dirent);
			pthread_mutex_lock(&recovery_lock);
    		wait_recovery_finish();
    		pthread_mutex_unlock(&recovery_lock);
		}
	}
	closedir(dir);
}

static void* recovery_process(void *arg){
	struct dirent *p_dirent=(struct dirent *)arg;
	char *key=strtok(p_dirent->d_name,".");
	int fd=open_queue_file(key);
	if(fd==-1){
		my_log(ERROR,"open queue file error\n");
		return NULL;
	}
	enum RECOVERY_STATUS status=START;
	char_t *dy_char=init_char();
	while(1){
		int count=read(fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
		if(count<=0){
			break;
		}
		rbuf->limit+=count;
		while(has_remaining(rbuf)){
			char c=*(rbuf->data+rbuf->current);
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
			rbuf->current+=1;
		}
		compact(rbuf);
	}
	remove_queue_file(key);
	close(fd);
	register_recovery_finish();
	reset(rbuf);
	return NULL;
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

static void trancate_queue_file(char *key){
	int fd=open_queue_file(key);
	int tmp_fd=open_queue_temp_file(key);
	off_t *o=(off_t *)get(key,sync_get_offset_hash);
	lseek(fd,*o,SEEK_SET);
	while(1){
		int count=read(fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
		if(count<=0){
			break;
		}
		rbuf->limit+=count;
		write(tmp_fd,rbuf->data+rbuf->current,rbuf->limit-rbuf->current);
		compact(rbuf);
	}
	close(fd);
	close(tmp_fd);
	if(rename(queue_temp_file_url(key),queue_file_url(key))<0){
		my_log(ERROR,"truncate queue file error\n");
	}
	reset(rbuf);
}
