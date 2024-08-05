#ifndef SPITFIRE_PARSE_H
#define SPITFIRE_PARSE_H

#include "lex.h"
#include "ast.h"

typedef struct parser {
    Token curr;
    Token next;
} Parser;

void parse_init(char *);
Stmt *parse(void);

#endif