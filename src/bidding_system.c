#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "utility.h"

int main(int argc, char *argv[]) {
    int host_num = atoi(argv[1]), player_num = atoi(argv[2]);

    int fd[20][2];
    pid_t pid;

    for (int i=0; i<host_num; i++) {
        if (pipe(fd[i]) < 0)
            err_sys("pipe error");
        if ((pid=fork()) < 0)
            err_sys("fork error");
        else if (pid == 0) {
            char tmp[10];
            sprintf(tmp, "%d", i+1);
            if (execl("host", tmp, NULL) < 0)
                err_sys("execl error");
        }
    }
    while (pid = waitpid(-1, NULL, 0))
        if (errno == ECHILD)
            break;
    return 0;
}
