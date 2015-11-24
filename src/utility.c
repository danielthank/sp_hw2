/*B02901107 金延儒*/
#include <stdlib.h>
#include <stdio.h>
#include "utility.h"

void err_sys(char *str) {
    fprintf(stderr, "%s\n", str);
    exit(-1);
}

int compare_score(const void *a, const void *b) {
    const Score *aa = (Score*)a;
    const Score *bb = (Score*)b;
    int tmp = bb->value - aa->value;
    if (!tmp) return aa->id - bb->id;
    return tmp;
}

