#include "c.h"

Stmt *ast_eof(void) {
    Stmt *s;
    NEW(s);

    s->type = STMT_EOF;
    s->stmt = NULL;

    return s;
}
Stmt *ast_null(void) {
    Stmt *s;
    NEW(s);

    s->type = STMT_NULL;
    s->stmt = NULL;

    return s;
}
Stmt *ast_expr(Expr *e) {
    Stmt *s;
    NEW(s);
    NEW(s->stmt);
    
    s->type = STMT_EXPR;
    s->stmt->expr = e;

    return s;
}
Expr *ast_infix(Expr *left, Expr *right, Token tok) {
    Expr *e;
    NEW(e);
    NEW(e->expr);

    e->type = EXPR_INFIX;
    e->expr->infix_expr.left = left;
    e->expr->infix_expr.right = right;
    e->expr->infix_expr.tok = tok;

    return e;
}
Expr *ast_id(Token tok) {
    Expr *e;
    NEW(e);
    NEW(e->expr);

    e->type = EXPR_IDENT;
    e->expr->id_expr.tok = tok;

    return e;
}
Expr *ast_int(Token tok) {
    Expr *e;
    NEW(e);
    NEW(e->expr);

    e->type = EXPR_INT;
    e->expr->int_expr.tok = tok;

    return e;
}