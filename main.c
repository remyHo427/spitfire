#define _GNU_SOURCE
#include "c.h"

#define INIT_BUF_SIZE   2048

void print_stmt(Stmt *);
void print_expr(Expr *);

int main(void) {
    Arena *arena = arena_new();
    char *l = malloc(INIT_BUF_SIZE);
    size_t buflen = INIT_BUF_SIZE;

    fputs("> ", stdout);
    while (getline(&l, &buflen, stdin) != -1) {
        parse_init(l, arena);
        print_stmt(parse());
        fputs("> ", stdout);
    }

    arena_free(&arena);
    arena_dispose();
    free(l);
#ifndef NDEBUG_MEM
    return dmem_check() > 0;
#else
    return 0;
#endif
}

void print_stmt(Stmt *s) {
    switch (s->type) {
    case STMT_NULL:
        printf("(null)\n");
        break;
    case STMT_EXPR:
        print_expr(s->stmt->expr);
        fputs("\n", stdout);
        break;
    case STMT_EOF:
        break;
    }
}

void print_expr(Expr *e) {
    switch (e->type) {
    case EXPR_INFIX:
        printf("(");
        print_expr(e->expr->infix_expr.left);
        printf(" %d ", e->expr->infix_expr.tok.type);
        print_expr(e->expr->infix_expr.right);
        printf(")");
        break;
    case EXPR_INT:
        printf("%ld", e->expr->int_expr.tok.val.i);
        break;
    case EXPR_IDENT:
        printf("id");
        break;
    default:
        break;
    }
}