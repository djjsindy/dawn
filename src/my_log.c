#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <string.h>
#include "my_log.h"
#include "memory.h"

static FILE *out;

extern jmp_buf exit_buf;

extern mem_pool_t *pool;

void my_log_init(){
 char dir[200];
 int length=strlen(PREFIX);
 memcpy(dir,PREFIX,length);
 char *log_dir="/log/dawn.log";
 int log_length=strlen(log_dir);
 memcpy(dir+length,log_dir,log_length);
 dir[length+log_length]='\0';
 if ((out = fopen(dir, "a+"))== NULL) { 
  fprintf(stderr,"%s\n","log file not found");
  longjmp(exit_buf,-1);
 }
}

void my_log(enum log_level level,char *format,...){
	if(level<LOG_LEVEL){
		return;
	}
	va_list ap;
	va_start(ap,format);
  time_t tt;
  char tmpbuf[80];     
  tt=time(NULL);
  strftime(tmpbuf,80,"%Y-%m-%d %H:%M:%S",localtime(&tt));
  fprintf(out,"%s ",tmpbuf);
  vfprintf(out,format,ap);
  fflush(out);
  va_end(ap);
}