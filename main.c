#include <time.h>
#include "c.h"

#define INIT_BUF_SIZE   2048

void print_stmt(Stmt *);
void print_expr(Expr *);

int main(void) {
    size_t len;
    FILE* f;
    char* buf;

    if ((f = fopen("test.txt", "r")) == NULL) {
        fprintf(stderr, "file not found\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = (char *)malloc(sizeof (char) * (len + 1));
    fread(buf, 1, len, f);
    fclose(f);
    buf[len] = '\0';

    clock_t start = clock();
    Arena *a;
    Stmt *s;

    parse_init(buf);
    while (1) {
        a = arena_new();
        s = parse(a);
        if ((s = parse(a))->type == STMT_EOF) {
            arena_free(&a);
            break;
        }
        arena_free(&a);
    }
    clock_t end = clock();

    printf("time: %f\n", (double)(end - start) / CLOCKS_PER_SEC));

    arena_dispose();
    free(buf);
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
        print_expr(s->expr);
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
        print_expr(e->left);
        printf(" %d ", e->tok.type);
        print_expr(e->right);
        printf(")");
        break;
    case EXPR_INT:
        printf("%ld", e->tok.val.i);
        break;
    case EXPR_IDENT:
        printf("id");
        break;
    default:
        break;
    }
}