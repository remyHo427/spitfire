#include "c.h"

int check(char *, Stmt *);
int check_stmts(Stmt *, Stmt *);
int check_expr(Expr *, Expr *);

int checkNullStmt(void);
int checkEofStmt(void);
int checkIdStmt(void);
int checkInfixStmt(void);

int main(void) {
    assert(checkNullStmt());
    assert(checkEofStmt());
    assert(checkIdStmt());
    assert(checkInfixStmt());

    // need to write a function to free trees
    // so dmem_check() doesn't yell at me
    dmem_check();
    return 0;
}

int checkNullStmt(void) {
    return check(";", ast_null());
}
int checkEofStmt(void) {
    return check("", ast_eof());
}
int checkIdStmt(void) {
    Token tok;

    tok.type = TOK_IDENT;
    tok.val.s.start = 0;
    tok.val.s.end = 0;

    return check("id;", ast_expr(ast_id(tok)));
}
int checkInfixStmt(void) {
    Token i1;
    Token i2;
    Token plus;

    i1.type = TOK_INT;
    i1.val.i = 1;
    i2.type = TOK_INT;
    i2.val.i = 1;
    plus.type = TOK_ADD;

    assert(check("1+1;", 
        ast_expr(
            ast_infix(
                ast_int(i1), 
                ast_int(i2), 
                plus))));

    return 1;
}

//
int check(char *s, Stmt *stmt) {
    parse_init(s);
    return check_stmts(parse(), stmt);
}
int check_stmts(Stmt *s1, Stmt *s2) {
    if (s1->type != s2->type) {
        return 0;
    }
    switch (s1->type) {
    case STMT_EXPR:
        return check_expr(s1->stmt->expr, s2->stmt->expr);
    case STMT_NULL:
        return 1;
    case STMT_EOF:
        return 1;
    default:
        return 0;
    }
}
int check_expr(Expr *e1, Expr *e2) {
    if (e1->type != e2->type) {
        return 0;
    }
    switch (e1->type) {
    case EXPR_IDENT:
        return 1;
    case EXPR_INFIX: {
        int same_left = 
            check_expr(e1->expr->infix_expr.left, e2->expr->infix_expr.left);
        int same_right =
            check_expr(e1->expr->infix_expr.right, e2->expr->infix_expr.right);
        int same_opt =
            e1->expr->infix_expr.tok.type == e2->expr->infix_expr.tok.type;

        return same_left && same_right && same_opt;
    }
    case EXPR_INT:
        return e1->expr->int_expr.tok.val.i == e2->expr->int_expr.tok.val.i;
    default:
        return 0;
    }
}
