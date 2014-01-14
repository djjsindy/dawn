#include <fcntl.h>
int set_noblocking(int fd){
  int flags=0;
  if((flags = fcntl(fd, F_GETFL)) < 0)
    return -1;
  flags|= O_NONBLOCK;
  if(fcntl(fd, F_SETFL, flags)<0)
    return -1;
  return 0;
}
