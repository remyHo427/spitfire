#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

#define INIT_BUF_SIZE   1048

int main(void) {
    size_t buflen = INIT_BUF_SIZE;
    char *l = malloc(buflen);

    printf("> ");
    while (getline(&l, &buflen, stdin) != -1) {
        parse_init(l);
        parse();
        printf("> ");
    }

    free(l);
    l = NULL;

    return 0;
}