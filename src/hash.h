#include <pthread.h>
struct hash_element_s{
  char *data;
  char *key;
  struct hash_element_t *next;
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

void put(char *key,char *data,hash_t *hash);

char* get(char *key,hash_t *hash);
