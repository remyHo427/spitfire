#ifndef SPITFIRE_C_H
#define SPITFIRE_C_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#ifndef NDEBUG_MEM
    // dmem.c
    #define malloc(s)   (tmalloc(s, __FILE__, __LINE__, __func__))
    #define free(p)     (tfree(p, __FILE__, __LINE__, __func__))

    void *tmalloc(size_t s, const char *, unsigned long, const char *);
    void  tfree(void *, const char *, unsigned long, const char *);

    int dmem_check(void);
#endif

// arena.c
#define NEW(p, a)   ((p) = arena_alloc(sizeof *(p), (a)))

typedef struct arena {
    char         *pool;
    struct arena *next;
    size_t        curr;
    size_t        max;
} Arena;

Arena *arena_new(void);
void   arena_free(Arena **);
void   arena_dispose(void);
void  *arena_alloc(size_t, Arena *);

// lex.c
typedef enum toktype {
    TOK_EOF = 0,
    TOK_RBRACE,
    TOK_LBRACE,
    TOK_RPAREN,
    TOK_LPAREN,
    TOK_RBRACKET,
    TOK_LBRACKET,
    TOK_NOT,
    TOK_NEQ,
    TOK_EQ,
    TOK_ASSIGN,
    TOK_ADD,
    TOK_ADD_ASSIGN,
    TOK_SUB,
    TOK_SUB_ASSIGN,
    TOK_MUL,
    TOK_MUL_ASSIGN,
    TOK_DIV,
    TOK_DIV_ASSIGN,
    TOK_AND,
    TOK_OR,
    TOK_SCOLON,
    TOK_COLON,
    TOK_QMARK,
    TOK_INC,
    TOK_DEC,
    TOK_LT,
    TOK_GT,
    TOK_LEQ,
    TOK_GEQ,
    TOK_IDENT,
    TOK_INT,
    TOK_IF,
    TOK_ELSE,
    TOK_VAR,
    TOK_FUNC,
    TOK_RETURN,
    TOK_WHILE,
    TOK_CONTINUE,
    TOK_BREAK,
} Toktype;

typedef struct str_slice {
    unsigned long start;
    unsigned long end;
} Strslice;

typedef union tokval {
    long i;
    char b;
    Strslice s;
} Tokval;

typedef struct lexer {
    char *src;
    unsigned long sp;
    unsigned long len;
} Lexer;

typedef struct tok {
    Toktype type;
    Tokval val;
} Token ;

void lex_init(char *src);
Token lex(void);

// ast.c
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
    Token tok;
    struct expr *left;
    struct expr *right;
} Expr;

typedef struct stmt {
    StmtType type;
    Expr *expr;
} Stmt;

// parse.c
typedef struct parser {
    Token curr;
    Token next;
} Parser;

void parse_init(char *);
Stmt *parse(Arena *);

#endif