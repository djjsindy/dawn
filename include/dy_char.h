#ifndef CHAR_H
#define CHAR_H
#define CHAR_SIZE 16
struct char_s{
  char *data;
  int size;
  int current;
};

typedef struct char_s char_t;

char_t *init_char();

void add_char(char_t *t,char c);

void add_chars(char_t *t,char *cs);

void add_terminal(char_t *t);

void reset_char(char_t *t);

void destroy_char(char_t *t);
#endif
