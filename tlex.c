#include <stdio.h>
#include <assert.h>
#include "lex.h"

int main(void) {
    Toktype seq[] = {
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
        TOK_LEQ,
        TOK_LT,
        TOK_GEQ,
        TOK_GT
    };
    lex_init(
        "} { ) ( ] [ ! != == = + += - -= * *= / /= && || "
        " ; : ? ++ -- a 0 if else var func return "
        "while continue break <= < >= >"
    );

    Token tok;
    int i = 0;
    while (i < 39 && (tok = lex()).type != TOK_EOF) {
        if (tok.type != seq[i]) {
            printf("%d failed, expect: %d, got: %d\n", i, seq[i], tok.type);
            break;
        }
        i++;
    }
    if (i != 39) {
        printf("failed, stopped at %d\n", i);
    }
}