#include <fcntl.h>
#include "utility.h"

int host_id, random_key, money_in[4];
char player_index;
char buf[100];

int main(int argc, char *argv[]) {
    host_id = atoi(argv[1]);
    player_index = argv[2][0]-'A';
    random_key = atoi(argv[3]);
    sprintf(buf, "host%d_%c.FIFO", host_id, argv[2][0]);
    int fdin = open(buf, O_RDONLY);
    FILE *fin = fdopen(fdin, "r");

    sprintf(buf, "host%d.FIFO", host_id);
    int fdout = open(buf, O_WRONLY | O_APPEND);
    FILE *fout = fdopen(fdout, "a");
    for (int round=0; round<10; round++) {
        for (int i=0; i<4; i++)
            fscanf(fin, "%d", money_in+i);
        int pay = 0;
        if (round % 4 == player_index)
            pay = money_in[player_index];
        fprintf(fout, "%c %d %d\n", argv[2][0], random_key, pay);
        fflush(fout);
        fsync(fdout);
    }
    return 0;
}
