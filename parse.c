#include "lex.h"
#include "parse.h"

#define IS(toktype) (p.curr.type == (toktype))

static Parser p;

void adv(void);

void parse_init(char *s) {
    lex_init(s);
    adv();
    adv();
}
void parse(void) {
    while (!IS(TOK_EOF)) {
        adv();
    }
}

void adv(void) {
    p.curr = p.next;
    p.next = lex();
}