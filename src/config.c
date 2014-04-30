#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "config.h"
#include "buffer.h"
#include "my_log.h"

#define CONF_FILE "/etc/dawn.conf"

#define CONF_READ_BUF 1024*4

static int open_conf_file();

static config_module_t *find_config_module(char_t *module_name);

static command_t *find_config_command(char_t *command_key,config_module_t *current_module);

static int char_is_special(char c);

extern config_module_t listen_conf_module;

extern config_module_t thread_conf_module;

extern config_module_t connection_conf_module;

extern config_module_t events_conf_module;

extern config_module_t hash_conf_module;

extern config_module_t pesistence_conf_module;

config_module_t *config_modules[]={
	&listen_conf_module,
	&thread_conf_module,
	&connection_conf_module,
	&events_conf_module,
	&hash_conf_module,
	&pesistence_conf_module,
	NULL
};

static const char brace_start='{';

static const char brace_end='}';

static const char space=' ';

static const char special_r='\r';

static const char special_n='\n';

static const char special_t='\t';

static const char semicolon=';';

enum PARSE_STATE{
	RAW,
	MODULE_START,
	FIND_COMMAND_KEY,
	COMMAND_KEY_START,
	FIND_COMMAND_VALUE,
	COMMAND_VALUE_START,
}; 

/**
 * 加载配置文件
 */
void init_conf(){
	buffer_t *rbuf=alloc_buffer(CONF_READ_BUF);
	int fd=open_conf_file();
	enum PARSE_STATE status=RAW;
	config_module_t *current_module;
	command_t *current_command;
	char_t *module_name=init_char();
	char_t *command_key=init_char();
	char_t *command_value=init_char();
	while(1){
		if(has_space(rbuf)){
			int count=read(fd,rbuf->data+rbuf->limit,rbuf->size-rbuf->limit);
			if(count<=0){
				goto CONFIG_END;
			}
			rbuf->limit+=count;
		}
		while(has_remaining(rbuf)){
    		char c=*(rbuf->data+rbuf->current);
    		rbuf->current+=1;
    		if(status==RAW&&c==brace_start){
    			status=MODULE_START;
    		}else if(status==MODULE_START&&!char_is_special(c)){
    			if(c==brace_start){
    				add_terminal(module_name);
    				current_module=find_config_module(module_name);
    				if(current_module==NULL){
    					my_log(ERROR,"config error,please check\n");
    					goto CONFIG_END;
    				}else{
    					status=FIND_COMMAND_KEY;
    				}
    			}else if(c==brace_end){
    				goto CONFIG_END;
    			}else{
    				add_char(module_name,c);
    			}
    		}else if(status==FIND_COMMAND_KEY&&!char_is_special(c)){
    			if(c==brace_end){
    				current_module=NULL;
    				reset_char(module_name);
    				status=MODULE_START;
    			}else{
    				add_char(command_key,c);
    				status=COMMAND_KEY_START;
    			}
    		}else if(status==COMMAND_KEY_START){
    			if(!char_is_special(c)){
    				add_char(command_key,c);
    			}else{
    				add_terminal(command_key);
    				current_command=find_config_command(command_key,current_module);
    				if(current_command==NULL){
    					my_log(ERROR,"config error,please check\n");
    					goto CONFIG_END;
    				}
    				status=COMMAND_VALUE_START;
    			}
				
    		}else if(status==COMMAND_VALUE_START&&!char_is_special(c)){
    			if(c!=semicolon){
    				add_char(command_value,c);
    			}else{
    				add_terminal(command_value);
    				printf("set value %s\n",command_value->data);
    				current_command->set_value(command_value);
    				current_command=NULL;
    				reset_char(command_value);
    				reset_char(command_key);
    				status=FIND_COMMAND_KEY;
    			}
    		}
    	}
	}
CONFIG_END:
	destroy_buffer(rbuf);
}

static int char_is_special(char c){
	if(c==special_r||c==special_n||c==special_t||c==space)
		return 1;
	return 0;
}

static int open_conf_file(){
	char_t *s=init_char();
	add_chars(s,DAWN_HOME);
	add_chars(s,CONF_FILE);
	add_terminal(s);
	int fd=open(s->data,O_RDWR);
	if(fd==-1){
		my_log(ERROR,"open config file error\n");
	}
	destroy_char(s);
	return fd;
}

static config_module_t *find_config_module(char_t *module_name){
	int index=0;
	while(*(config_modules+index)!=NULL){
		config_module_t *c_module=*(config_modules+index);
		if(strcmp(c_module->name,module_name->data)==0){
			return c_module;
		}
		index++;
	}
	return NULL; 
}

static command_t *find_config_command(char_t *command_key,config_module_t *current_module){
	command_t *commands=current_module->commands;
	int index=0;
	while(commands+index!=NULL){
		command_t cm=*(commands+index);
		if(strcmp(cm.name,command_key->data)==0){
			return commands+index;
		}
		index++;
	}
	return NULL;
}


