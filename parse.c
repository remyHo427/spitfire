#include "lex.h"
#include "parse.h"
#include "ast.h"

#define IS(toktype) (p.curr.type == (toktype))
#define PEEK()      (p.curr.type)

static Parser p;

Stmt *parseStmt(void);
Stmt *parseNullStmt(void);
void adv(void);

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
        return NULL;
    }
}
Stmt *parseNullStmt(void) {
    Stmt *s;
    NEW(s);
    
    s->type = STMT_NULL;
    s->stmt = NULL;

    return s;
}

void adv(void) {
    p.curr = p.next;
    p.next = lex();
}