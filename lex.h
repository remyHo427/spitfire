#ifndef SPITFIRE_LEX_H
#define SPITFIRE_LEX_H

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

#endif