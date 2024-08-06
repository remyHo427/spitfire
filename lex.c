#include "c.h"

#define ADV()       (l.sp++)
#define IS_END()    (l.sp >= l.len)
#define PEEK(c)     ((c) = (l.src[l.sp]))
#define SPEEK(c)    ((c) = (IS_END() ? '\0' : l.src[l.sp]))

static Lexer l;

static inline Token tok(Toktype);
Toktype iskeyword(unsigned long start, unsigned long end);
int cmp(unsigned long start, unsigned long end, char *s);

void lex_init(char *src) {
    l.src = src;
    l.sp = 0;
    l.len = strlen(src);
}

Token lex(void) {
    register int c;

    while (!IS_END()) {
        if (isspace(PEEK(c))) {
            ADV();
            continue;
        } else if (isalpha(c)) {
            Token token;
            Toktype type;
            unsigned long start, end;
            start = l.sp;

            do {
                ADV();
            } while (!IS_END() && isalnum(PEEK(c)));
            end = l.sp;

            if ((type = iskeyword(start, end)) != TOK_EOF) {
                return tok(type);
            }

            token.type = TOK_IDENT;
            token.val.s.start = start;
            token.val.s.end = end;

            return token;
        } else if (isdigit(c)) {
            Token token;
            unsigned long start, end;
            unsigned long n;
            start = l.sp;

            if (c == '0') {
                ADV();
                token.type = TOK_INT;
                token.val.i = 0;
                return token;
            }

            do {
                ADV();
            } while(!IS_END() && isdigit(PEEK(c)));
            end = l.sp;

            n = 0;
            for (unsigned long i = start; i < end; i++) {
                n *= 10;
                n += l.src[i] - '0';
            }

            token.type = TOK_INT;
            token.val.i = n;

            return token;
        }
        switch (c) {
        case '}':
            ADV();
            return tok(TOK_RBRACE);
        case '{':
            ADV();
            return tok(TOK_LBRACE);
        case ')':
            ADV();
            return tok(TOK_RPAREN);
        case '(':
            ADV();
            return tok(TOK_LPAREN);
        case ']':
            ADV();
            return tok(TOK_RBRACKET);
        case '[':
            ADV();
            return tok(TOK_LBRACKET);
        case '?':
            ADV();
            return tok(TOK_QMARK);
        case ':':
            ADV();
            return tok(TOK_COLON);
        case ';':
            ADV();
            return tok(TOK_SCOLON);
        case '>':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_GEQ);
            } else {
                return tok(TOK_GT);
            }
        case '<':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_LEQ);
            } else {
                return tok(TOK_LT);
            }
        case '&':
            ADV();
            if (SPEEK(c) == '&') {
                ADV();
                return tok(TOK_AND);
            }
            continue;
        case '|':
            ADV();
            if (SPEEK(c) == '|') {
                ADV();
                return tok(TOK_OR);
            }
            continue;
        case '!':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_NEQ);
            } else {
                return tok(TOK_NOT);
            }
        case '=':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_EQ);
            } else {
                return tok(TOK_ASSIGN);
            }
        case '+':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_ADD_ASSIGN);
            } else if (c == '+') {
                ADV();
                return tok(TOK_INC);
            } else {
                return tok(TOK_ADD);
            }
        case '-':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_SUB_ASSIGN);
            } else if (c == '-') {
                ADV();
                return tok(TOK_DEC);
            } else {
                return tok(TOK_SUB);
            }
        case '*':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_MUL_ASSIGN);
            } else {
                return tok(TOK_MUL);
            }
        case '/':
            ADV();
            if (SPEEK(c) == '=') {
                ADV();
                return tok(TOK_DIV_ASSIGN);
            } else {
                return tok(TOK_DIV);
            }
        case '#':
            do {
                ADV();
            } while (!IS_END() && PEEK(c) != '\n');
            continue;
        default:
            ADV();
            continue;
        }
    }

    return tok(TOK_EOF);
}

static inline Token tok(Toktype type) {
    Token tok;
    tok.type = type;
    return tok;
}

Toktype iskeyword(unsigned long start, unsigned long end) {
    unsigned long len = end - start;

    if (len > 8) {
        return TOK_EOF;
    }

    switch (l.src[start]) {
    case 'i':
        return cmp(start+1, end, "f") ? TOK_IF : TOK_EOF;
    case 'e':
        return cmp(start+1, end, "lse") ? TOK_ELSE : TOK_EOF;
    case 'v':
        return cmp(start+1, end, "ar") ? TOK_VAR : TOK_EOF;
    case 'f':
        return cmp(start+1, end, "unc") ? TOK_FUNC : TOK_EOF;
    case 'r':
        return cmp(start+1, end, "eturn") ? TOK_RETURN : TOK_EOF;   
    case 'w':
        return cmp(start+1, end, "hile") ? TOK_WHILE : TOK_EOF;
    case 'c':
        return cmp(start+1, end, "ontinue") ? TOK_CONTINUE : TOK_EOF;
    case 'b':
        return cmp(start+1, end, "reak") ? TOK_BREAK : TOK_EOF;
    default:
        return TOK_EOF;
    }
}

int cmp(unsigned long start, unsigned long end, char *s) {
    if (end - start != strlen(s)) {
        return 0;
    }

    for (unsigned long j = 0; start < end; ) {
        if (l.src[start++] != s[j++]) {
            return 0;
        }
    }

    return 1;
}