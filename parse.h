#ifndef SPITFIRE_PARSE_H
#define SPITFIRE_PARSE_H

#include "lex.h"

typedef struct parser {
    Token curr;
    Token next;
} Parser;

void parse_init(char *);
void parse(void);

#endif