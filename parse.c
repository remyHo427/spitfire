#include "c.h"

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
static Arena *a;

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

Stmt* parse(Arena *arena) {
    Stmt *s;

    a = arena;
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

    NEW(s, a);
    s->type = STMT_EOF;

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
    NEW(s, a);
    
    s->type = STMT_NULL;

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

    NEW(s, a);
    s->type = STMT_EXPR;
    s->expr = e;

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
    switch (PEEK()) {
    case TOK_IDENT:
        NEW(e, a);
        e->type = EXPR_IDENT;
        e->tok = p.curr;
        return e;
    case TOK_INT:
        NEW(e, a);
        e->type = EXPR_INT;
        e->tok = p.curr;
        return e;
    default:
        return NULL;
    }
}

Expr *parseInfix(Expr *left) {
    Expr *e;
    Expr *right;
    Token tok;
    Prec currPrec;

    tok = p.curr;
    currPrec = prec(tok.type);
    adv();

    if ((right = parseExpr(currPrec)) == NULL) {
        return NULL;
    }

    NEW(e, a);
    e->type = EXPR_INFIX;
    e->left = left;
    e->right = right;
    e->tok = tok;

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