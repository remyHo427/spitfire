#include <stdio.h>
#include "lex.h"
#include "parse.h"
#include "ast.h"

#define IS(toktype) (p.curr.type == (toktype))
#define PEEK()      (p.curr.type)
#define PEEKN()     (p.next.type)

typedef enum prec_enum {
    LOWEST = 0,
    ASSIGN,
    COND,
    OR,
    AND,
    EQ,
    ORDER,
    SUM,
    PRODUCT,
    PREFIX,
    POSTFIX
} Prec;

static Parser p;

Stmt *parseStmt(void);
Stmt *parseNullStmt(void);
Stmt *parseExprStmt(void);
Expr *parseExpr(Prec);
Expr *parsePrefix(void);
Expr *parseInfix(Expr *);

void adv(void);
int expect(Toktype);
Prec prec(Toktype);

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

    if ((e = parseExpr(LOWEST)) == NULL) {
        return NULL;
    }
    adv();

    if (!expect(TOK_SCOLON)) {
        return NULL;
    }
    adv();

    NEW(s);
    NEW(s->stmt);
    s->type = STMT_EXPR;
    s->stmt->expr = e;
    return s;    
}

Expr *parseExpr(Prec currPrec) {
    Expr *left;

    if ((left = parsePrefix()) == NULL) {
        return NULL;
    }

    while (!IS(TOK_SCOLON) && currPrec < prec(PEEKN())) {
        adv();

        switch (PEEK()) {
        case TOK_ADD:
        case TOK_SUB:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_EQ:
        case TOK_NEQ:
        case TOK_AND:
        case TOK_OR:
            left = parseInfix(left);
        default:
            break;
        }
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

Expr *parseInfix(Expr *left) {
    Prec currPrec;
    Expr *e;
    Expr *right;
    Token tok;

    tok = p.curr;
    currPrec = prec(tok.type);
    adv();

    if ((right = parseExpr(currPrec)) == NULL) {
        return NULL;
    }

    NEW(e);
    NEW(e->expr);
    e->type = EXPR_INFIX;
    e->expr->infix_expr.left = left;
    e->expr->infix_expr.right = right;
    e->expr->infix_expr.tok = tok;

    return e;
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

Prec prec(Toktype type) {
    switch (type) {
    case TOK_INC:
    case TOK_DEC:
    case TOK_LPAREN:
    case TOK_LBRACKET:
        return POSTFIX;
    case TOK_MUL:
    case TOK_DIV:
        return PRODUCT;
    case TOK_ADD:
    case TOK_SUB:
        return SUM;
    case TOK_EQ:
    case TOK_NEQ:
        return EQ;
    case TOK_AND:
        return AND;
    case TOK_OR:
        return OR;
    case TOK_QMARK:
        return COND;
    case TOK_ADD_ASSIGN:
    case TOK_SUB_ASSIGN:
    case TOK_MUL_ASSIGN:
    case TOK_DIV_ASSIGN:
    case TOK_ASSIGN:
        return ASSIGN;
    default:
        return LOWEST;
    }
}