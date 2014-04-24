#include "dy_char.h"
struct command_s{
  char *name;
  void (*set_value)(char_t *value);
};

typedef struct command_s command_t;

struct config_module_s{
  char *name;
  command_t *commands; 
};

typedef struct config_module_s config_module_t;

void init_conf();
