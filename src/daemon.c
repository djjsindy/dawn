#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "my_log.h"
int daemonize()
{
    intptr_t fd;

    switch (fork()) {
    case -1:
        return (-1);
    case 0:
        break;
    default:
        exit(0);
    }

    if (setsid() == -1)
        return -1;

    if(chdir("/") != 0) {
        my_log(ERROR,"chdir error\n");
        return -1;
    }

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        if(dup2(fd, STDIN_FILENO) < 0) {
            my_log(ERROR,"dup2 stdin error\n");
            return (-1);
        }
        if(dup2(fd, STDOUT_FILENO) < 0) {
            my_log(ERROR,"dup2 stdout error\n");
            return (-1);
        }
        if(dup2(fd, STDERR_FILENO) < 0) {
            my_log(ERROR,"dup2 stderr error\n");
            return (-1);
        }

        if (fd > STDERR_FILENO) {
            if(close(fd) < 0) {
                my_log(ERROR,"close error\n");
                return (-1);
            }
        }
    }
    return (0);
}
