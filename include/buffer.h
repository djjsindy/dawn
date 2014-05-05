#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>
struct buffer_s{
  char *data;
  intptr_t size;
  intptr_t current;
  intptr_t limit;
};

typedef struct buffer_s buffer_t;

buffer_t *alloc_buffer(intptr_t size);

void compact(buffer_t *b);

void reset(buffer_t *b);

intptr_t has_space(buffer_t *b);

intptr_t has_remaining(buffer_t *b);

void destroy_buffer(buffer_t *b);
#endif
