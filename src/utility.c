#include <stdlib.h>
#include <stdio.h>

void err_sys(char *str) {
    fprintf(stderr, "%s\n", str);
    exit(-1);
}
