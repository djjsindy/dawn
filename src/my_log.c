#include <stdio.h>
#include <setjmp.h>
#include "my_log.h"
#include <time.h>

static FILE *out;

extern jmp_buf exit_buf;

void my_log_init(){
   if ((out = fopen(LOG_FILE, "a+"))== NULL) { 
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