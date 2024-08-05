#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "ast.h"

#define INIT_BUF_SIZE   1048

void print_stmt(Stmt *);

int main(void) {
    size_t buflen = INIT_BUF_SIZE;
    char *l = malloc(buflen);

    printf("> ");
    while (getline(&l, &buflen, stdin) != -1) {
        parse_init(l);
        print_stmt(parse());
        printf("> ");
    }

    free(l);
    l = NULL;

    return 0;
}

void print_stmt(Stmt *s) {
    switch (s->type) {
    case STMT_NULL:
        printf("(null)\n");
        break;
    case STMT_EXPR:
        printf("(expr)");
        break;
    case STMT_EOF:
        break;
    }

    free(s);
    s = NULL;
}