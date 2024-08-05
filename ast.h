#ifndef SPITFIRE_AST_H
#define SPITFIRE_AST_H

#include <stdlib.h>

#define NEW(p)   ((p) = malloc(sizeof *(p)))

typedef enum stmttype{
    STMT_EOF = 0,
    STMT_NULL,
    STMT_EXPR
} StmtType;

typedef enum exprtype {
    EXPR_INT = 0,
    EXPR_IDENT
} ExprType;

typedef struct expr {
    ExprType type;
    union {
        struct {
            long value;
        } int_expr;
        struct {
            char *name;
        } id_expr;
    } *expr;
} Expr;

typedef struct stmt {
    StmtType type;
    union {
        struct {
            Expr expr;
        } expr_stmt;
    } *stmt;
} Stmt;

Expr *new_int_expr(long value);

#endif