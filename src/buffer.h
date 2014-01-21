#ifndef BUFFER_H
#define BUFFER_H
struct buffer_s{
  char *data;
  int size;
  int current;
  int limit;
};

typedef struct buffer_s buffer_t;

buffer_t *alloc_buffer(int size);

void compact(buffer_t *b);
#endif