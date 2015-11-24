#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

typedef struct {
    int id;
    int value;
}Score;

void err_sys(char*);

int compare_score(const void*, const void*);

