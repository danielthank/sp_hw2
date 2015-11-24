#include "utility.h"

int fd[12][4];
int fd_r2w[100];
char comb[5000][20];
int place[21];
int host_num, player_num;
int cnt;

Score score[20];

void next_permutation(int index, int num, int n, int *solution) {
    if (index == 4) {
        char buf[100];
        sprintf(comb[cnt++], "%d %d %d %d\n", solution[0]+1, solution[1]+1, solution[2]+1, solution[3]+1);
        return;
    }
    for (int i=num; i<n; i++) {
        solution[index] = i;
        next_permutation(index+1, i+1, n, solution);
    }
}

void add_score(int fd) {
    FILE *f = fdopen(fd, "r");
    int id, rank;
    for (int i=0; i<4; i++) {
        fscanf(f, "%d%d", &id, &rank);
        score[id-1].value += (4-rank);
    }
}

int main(int argc, char *argv[]) {
    host_num = atoi(argv[1]);
    player_num = atoi(argv[2]);

    pid_t pid;

    for (int i=0; i<player_num; i++) score[i].id = i+1, score[i].value = 0;

    int maxfd = 0;
    for (int i=0; i<host_num; i++) {
        if (pipe(&fd[i][0]) < 0)
            err_sys("pipe error");
        if (pipe(&fd[i][2]) < 0)
            err_sys("pipe error");
        fd_r2w[fd[i][0]] = fd[i][3];
        if ((pid=fork()) < 0)
            err_sys("fork error");
        else if (pid == 0) {
            close(fd[i][0]);
            close(fd[i][3]);
            dup2(fd[i][1], STDOUT_FILENO);
            dup2(fd[i][2], STDIN_FILENO);
            char tmp[10];
            sprintf(tmp, "%d", i+1);
            if (execl("host", "./host", tmp, (char*)0) < 0)
                err_sys("execl error");
            exit(0);
        }
        close(fd[i][1]);
        close(fd[i][2]);
        if (fd[i][0] >= maxfd) maxfd = fd[i][0]+1;
    }
    int solution[4];
    next_permutation(0, 0, player_num, solution);

    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    int now = 0;
    char buf[1000];
    int complete = 0;
    while (1) {
        if (now < host_num && now < cnt) {
            write(fd[now][3], comb[now], strlen(comb[now]));
            fsync(fd[now][3]);
            FD_SET(fd[now][0], &master_set);
            now++;
        }
        else {
            memcpy(&working_set, &master_set, sizeof(master_set));
            int ok;
            select(maxfd, &working_set, NULL, NULL, NULL);
            for (ok=0; ok<maxfd; ok++) {
                if(FD_ISSET(ok, &working_set)) {
                    add_score(ok);
                    complete++;
                    if (now < cnt) {
                        write(fd_r2w[ok], comb[now], strlen(comb[now]));
                        fsync(fd_r2w[ok]);
                        now++;
                    }
                    else {
                        FD_CLR(ok, &master_set);
                        if (complete == cnt) break;
                    }
                }
            }
            if (ok < maxfd) break;
        }
    }

    strcpy(buf, "-1 -1 -1 -1\n");
    for (int i=0; i<host_num ;i++) {
        write(fd[i][3], buf, strlen(buf));
        fsync(fd[i][3]);
    }

    qsort((void*)score, player_num, sizeof(Score), compare_score);
    place[score[0].id] = 1;
    for (int i=1; i<player_num; i++) {
        if (score[i].value == score[i-1].value)
            place[score[i].id] = place[score[i-1].id];
        else
            place[score[i].id] = i+1;
    }
    for (int i=1; i<=player_num; i++)
        printf("%d %d\n", i, place[i]);

    while (pid = waitpid(-1, NULL, 0))
        if (errno == ECHILD)
            break;
    return 0;
}
