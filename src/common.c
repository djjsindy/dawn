#include <stdlib.h>
#include <stdio.h>
#include <dy_char.h>
int parse_int(char_t *s){
	char c=*(s->data+s->current-1);
	int radio=1;
	switch(c){
		case 'k':
		case 'K':
			radio*=1024;
			*(s->data+s->current-1)='\0';
			break;
		case 'm':
		case 'M':
			radio*=1024;
			radio*=1024;
			*(s->data+s->current-1)='\0';
			break;
	}
	return atoi(s->data);
}