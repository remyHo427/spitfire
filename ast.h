#ifndef SPITFIRE_AST_H
#define SPITFIRE_AST_H

#include <stdlib.h>
#include "lex.h"

#define NEW(p)   ((p) = malloc(sizeof *(p)))

typedef enum stmttype{
    STMT_EOF = 0,
    STMT_NULL,
    STMT_EXPR
} StmtType;

typedef enum exprtype {
    EXPR_INT = 0,
    EXPR_IDENT,
    EXPR_INFIX
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
        struct {
            Token tok;
            struct expr *left;
            struct expr *right;
        } infix_expr;
    } *expr;
} Expr;

typedef struct stmt {
    StmtType type;
    union {
        Expr *expr;
    } *stmt;
} Stmt;

Expr *new_int_expr(long value);

#endif