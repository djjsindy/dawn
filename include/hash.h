#ifndef HASH_H
#define HASH_H
#include <pthread.h>
#define HASH_FACTOR 0.75f
#define INIT_SIZE 4
struct hash_element_s{
  void *data;
  char *key;
  struct hash_element_s *next;
};

typedef struct hash_element_s hash_element_t;

struct hash_s{
  hash_element_t **elements;
  int size;
  int num;
  pthread_mutex_t *mutex;
};

typedef struct hash_s hash_t;

hash_t* init_hash();

int put(char *key,void *data,hash_t *hash);

void* get(char *key,hash_t *hash);

void delete(char *key,hash_t *hash);
#endif
