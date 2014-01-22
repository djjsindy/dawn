#ifndef CHAR_H
#define CHAR_H
struct char_s{
  char *data;
  int size;
  int current;
};

typedef struct char_s char_t;

char_t *init_char(int size);

void add_char(char_t *t,char c);

void add_terminal(char_t *t);

void reset_char(char_t *t);
#endif
