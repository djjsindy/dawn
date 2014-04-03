#include <stdarg.h>
#define LOG_FILE "../log/dawn.log"
#define LOG_LEVEL INFO

enum log_level{
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
};

void my_log_init();

void my_log(enum log_level level,char *format,...);