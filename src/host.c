#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "utility.h"

int player_id[4], fd[5], randkey[4], money[4], cnt[4], place[4];
const char *player_index[4] = {"A", "B", "C", "D"};
char filename[5][50];
Score score[4];

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int host_id = atoi(argv[1]);
    char buf[100];
    pid_t pid;

    sprintf(filename[0], "host%d_A.FIFO", host_id);
    sprintf(filename[1], "host%d_B.FIFO", host_id);
    sprintf(filename[2], "host%d_C.FIFO", host_id);
    sprintf(filename[3], "host%d_D.FIFO", host_id);
    sprintf(filename[4], "host%d.FIFO", host_id);

    while (1) {
        mkfifo(filename[0], 0644);
        mkfifo(filename[1], 0644);
        mkfifo(filename[2], 0644);
        mkfifo(filename[3], 0644);
        mkfifo(filename[4], 0644);
        for (int i=0; i<4; i++)
            scanf("%d", player_id + i);
        if (player_id[0] == -1) break;
        for (int i=0; i<4; i++) {
            randkey[i] = rand()%65536;
            if ((pid=fork()) < 0)
                err_sys("fork error");
            else if (pid == 0) {
                sprintf(buf, "%d", randkey[i]);
                if (execl("player", "./player", argv[1], player_index[i], buf, (char*)0) < 0)
                    err_sys("execl error");
            }
            money[i] = 0;
            score[i].id = i;
            score[i].value = 0;
        }
        for (int i=0; i<4; i++)
            fd[i] = open(filename[i], O_WRONLY);
        fd[4] = open(filename[4], O_RDONLY);
        FILE* f4 = fdopen(fd[4], "r");
        for (int round=0; round<10; round++) {
            for (int i=0; i<4; i++) money[i] += 1000;
            sprintf(buf, "%d %d %d %d\n", money[0], money[1], money[2], money[3]);
            for (int i=0; i<4; i++) {
                write(fd[i], buf, strlen(buf));
                fsync(fd[i]);
            }
            int money_in[4], random_in, tmp;
            char index_in;
            for (int i=0; i<4; i++) {
                fgets(buf, sizeof(buf), f4);
                sscanf(buf, "%c%d%d", &index_in, &random_in, &tmp);
                index_in -= 'A';
                if (random_in != randkey[index_in])
                    fprintf(stderr, "rand different\n");
                else
                    money_in[index_in] = tmp;
            }
            int max = -1, max_index = -1, ok;
            for (int i=0; i<4; i++) {
                if (money_in[i] > max) {
                    max = money_in[i];
                    max_index = i;
                    ok = 1;
                }
                else if (money_in[i] == max) {
                    ok = 0;
                }
            }
            if (ok) {
                score[max_index].value++;
                money[max_index] -= money_in[max_index];
            }
        }
        qsort((void*)score, 4, sizeof(Score), compare_score);
        place[score[0].id] = 1;
        for (int i=1; i<4; i++) {
            if (score[i].value == score[i-1].value)
                place[score[i].id] = place[score[i-1].id];
            else
                place[score[i].id] = i+1;
        }
        sprintf(buf, "%d %d\n%d %d\n%d %d\n%d %d\n", player_id[0], place[0], player_id[1], place[1], player_id[2], place[2], player_id[3], place[3]);

        write(STDOUT_FILENO, buf, strlen(buf));
        fsync(STDOUT_FILENO);

        while(pid = waitpid(-1, NULL, 0))
            if (errno == ECHILD)
                break;
        for (int i=0; i<5; i++)
            close(fd[i]);
    }
    for (int i=0; i<5; i++)
        remove(filename[i]);
    return 0;
}
