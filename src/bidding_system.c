#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "utility.h"

void next_permutation(int index, int num, int n, int m, int *solution) {
    if (index == m) {
        char buf[20];
        sprintf(buf, "%d %d %d %d\n", solution[0]+1, solution[1]+1, solution[2]+1, solution[3]+1);
        printf("%s", buf);
        return;
    }
    for (int i=num; i<n; i++) {
        solution[index] = i;
        next_permutation(index+1, i+1, n, m, solution);
    }
}

int main(int argc, char *argv[]) {
    int host_num = atoi(argv[1]), player_num = atoi(argv[2]);

    int fd[20][2];
    pid_t pid;

    int score[4];
    for (int i=0; i<4; i++) score[i] = 0;

    for (int i=0; i<host_num; i++) {
        if (pipe(fd[i]) < 0)
            err_sys("pipe error");
        if ((pid=fork()) < 0)
            err_sys("fork error");
        else if (pid == 0) {
            char tmp[10];
            sprintf(tmp, "%d", i+1);
            printf("%s\n", tmp);
            if (execl("host", tmp, NULL) < 0)
                err_sys("execl error");
        }
    }
    int solution[10];
    next_permutation(0, 0, player_num, 4, solution);

    printf("parent end\n");
    while (pid = waitpid(-1, NULL, 0))
        if (errno == ECHILD)
            break;
    printf("wait end\n");
    return 0;
}
