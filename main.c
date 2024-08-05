#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "ast.h"

#define INIT_BUF_SIZE   1048

void print_stmt(Stmt *);
void print_expr(Expr *);

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
        printf("(expr ");
        print_expr(s->stmt->expr);
        printf(")\n");
        break;
    case STMT_EOF:
        break;
    }

    free(s);
}

void print_expr(Expr *e) {
    switch (e->type) {
    case EXPR_INT:
        printf("%ld", e->expr->int_expr.value);
        break;
    case EXPR_IDENT:
        printf("%s", e->expr->id_expr.name);
        break;
    default:
        break;
    }
    
    free(e);
}