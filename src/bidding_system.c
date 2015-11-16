#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utility.h"

int main(int argc, char *argv[]) {
    int host_num = atoi(argv[1]), player_num = atoi(argv[2]);

    int fd[20][2];
    pid_t pid;

    for (int i=0; i<host_num; i++) {
        if (pipe(fd[i]) < 0)
            err_sys("pipe error");
    }
    printf("%d %d\n", host_num, player_num);
    return 0;
}
