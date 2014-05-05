#include <fcntl.h>
#include <stdint.h>
int set_noblocking(int fd){
  intptr_t flags=0;
  if((flags = fcntl(fd, F_GETFL)) < 0)
    return -1;
  flags|= O_NONBLOCK;
  if(fcntl(fd, F_SETFL, flags)<0)
    return -1;
  return 0;
}
