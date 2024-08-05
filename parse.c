#include <stdio.h>
#include "lex.h"
#include "parse.h"
#include "ast.h"

#define IS(toktype) (p.curr.type == (toktype))
#define PEEK()      (p.curr.type)

static Parser p;

Stmt *parseStmt(void);
Stmt *parseNullStmt(void);
Stmt *parseExprStmt(void);
Expr *parseExpr(void);
Expr *parsePrefix(void);

void adv(void);
int expect(Toktype);

void parse_init(char *s) {
    lex_init(s);
    adv();
    adv();
}

Stmt* parse(void) {
    Stmt *s;

    while (!IS(TOK_EOF)) {
        if ((s = parseStmt()) == NULL) {
            while (!IS(TOK_SCOLON) && !IS(TOK_EOF)) {
                adv();
            }
            adv();
        } else {
            return s;
        }
    }

    NEW(s);
    s->type = STMT_EOF;
    s->stmt = NULL;

    return s;
}

Stmt *parseStmt(void) {
    switch (PEEK()) {
    case TOK_SCOLON:
        return parseNullStmt();
    default:
        return parseExprStmt();
    }
}
Stmt *parseNullStmt(void) {
    Stmt *s;
    NEW(s);
    
    s->type = STMT_NULL;
    s->stmt = NULL;

    adv();
    return s;
}
Stmt *parseExprStmt(void) {
    Stmt *s;
    Expr *e;

    NEW(s);
    s->type = STMT_EXPR;

    if ((e = parseExpr()) == NULL) {
        return NULL;
    }
    adv();

    if (!expect(TOK_SCOLON)) {
        return NULL;
    }
    adv();

    NEW(s->stmt);
    s->stmt->expr = e;
    return s;    
}

Expr *parseExpr(void) {
    Expr *left;

    if ((left = parsePrefix()) == NULL) {
        return NULL;
    }

    return left;
}

Expr *parsePrefix(void) {
    Expr *e;
    NEW(e);
    NEW(e->expr);

    switch (PEEK()) {
    case TOK_IDENT:
        e->type = EXPR_IDENT;
        e->expr->id_expr.name = "id";
        return e;
    case TOK_INT:
        e->type = EXPR_INT;
        e->expr->int_expr.value = p.curr.val.i;
        return e;
    default:
        return NULL;
    }
}

void adv(void) {
    p.curr = p.next;
    p.next = lex();
}

int expect(Toktype type) {
    if (IS(type)) {
        return 1;
    } else {
        fprintf(stderr, "expect %d, got %d\n", type, PEEK());
        return 0;
    }
}