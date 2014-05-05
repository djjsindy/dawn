#ifndef HASH_H
#define HASH_H
#include <pthread.h>
#include <stdint.h>
#include "list.h"
#define HASH_FACTOR 0.75f
#define INIT_SIZE 4
struct hash_element_s{
  void *data;
  char *key;
  struct hash_element_s *next;
};

typedef struct hash_element_s hash_element_t;

struct hash_entry_s{
  void *data;
  char *key;
  list_head_t list;
};

typedef struct hash_entry_s hash_entry_t;

struct hash_s{
  hash_element_t **elements;
  intptr_t size;
  intptr_t num;
  pthread_mutex_t *mutex;
};

typedef struct hash_s hash_t;

hash_t* init_hash();

intptr_t put(char *key,void *data,hash_t *hash);

void* get(char *key,hash_t *hash);

void delete(char *key,hash_t *hash);

list_head_t* visit_hash(hash_t *hash);

#endif
