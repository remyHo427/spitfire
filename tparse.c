#include "c.h"
#include <stdarg.h>

char *print_ast(char *s);
char *tokstr(Token);
size_t snprintf_node(char *, ...);
size_t snprintf_expr(Expr *);
size_t snprintf_stmt(Stmt *);
char *sprintf_node(char *, char *, ...);
char *sprintf_expr(char *, Expr *);
char *sprintf_stmt(char *, Stmt *);

int main(void) {
    char *s = print_ast("1;");
    printf("%s\n", s);
    free(s);

    dmem_check();
    return 0;
}

char *print_ast(char *s) {
    char *str;
    char *start;
    Stmt *ast;
    size_t len;

    parse_init(s);
    ast = parse();
    len = snprintf_node("%s", ast);

    start = str = malloc(sizeof (char) * (len + 1));
    str[len] = '\0';
    str = sprintf_node(str, "%s", ast);

    return start;
}

size_t snprintf_node(char *fmt, ...) {
    va_list np;
    char *p;
    char c;
    size_t result = 0;
    Stmt *s;
    Expr *e;

    va_start(np, fmt);
    for (p = fmt; (c = *p); p++) {
        result++;
        if (c != '%') {
            continue;
        }
        switch ((c = *++p)) {
        case 'e':
            e = va_arg(np, Expr *);
            result += snprintf_expr(e);
            break;
        case 's':
            s = va_arg(np, Stmt *);
            result += snprintf_stmt(s);
            break;
        default:
            result++;
            break;
        }
    }
    va_end(np);

    return result;
}

size_t snprintf_stmt(Stmt *s) {
    switch (s->type) {
    case STMT_NULL:
        return strlen("(null)");
    case STMT_EXPR:
        return snprintf_expr(s->stmt->expr);
    case STMT_EOF:
        return 0;
    default:
        return 0;
    }
}
size_t snprintf_expr(Expr *e) {
    switch (e->type) {
    case EXPR_IDENT:
        return 2;
    case EXPR_INFIX:
        return snprintf_expr(e->expr->infix_expr.left)
            +  snprintf_expr(e->expr->infix_expr.right)
            +  strlen(tokstr(e->expr->infix_expr.tok));
    case EXPR_INT:
        return 3;
    default:
        return 0;
    }
}
char *sprintf_node(char *s, char *fmt, ...) {
    char c;
    char *fmtp;
    va_list np;
    Stmt *stmt;
    Expr *expr;

    va_start(np, fmt);
    for (fmtp = fmt; (c = *fmtp); fmtp++) {
        if (c != '%') {
            *s = c;
            s++;
            continue;
        }
        switch ((c = *++fmtp)) {
        case 'e':
            expr = va_arg(np, Expr *);
            s = sprintf_expr(s, expr);
            break;
        case 's':
            stmt = va_arg(np, Stmt *);
            s = sprintf_stmt(s, stmt);
            break;
        default:
            *s = c;
            s++;
            continue;
        }
    }
    va_end(np);

    return s;
}
char *sprintf_expr(char *s, Expr *e) {
    switch (e->type) {
    case EXPR_IDENT:
        strncpy(s, "id", 2);
        s += 2;
        break;
    case EXPR_INT:
        strncpy(s, "INT", 3);
        s += 3;
        break;
    default:
        break;
    }

    free(e->expr);
    free(e);
    return s;
}
char *sprintf_stmt(char *s, Stmt *stmt) {
    switch (stmt->type) {
    case STMT_NULL:
        strncpy(s, "(null)", 6);
        s += 6;
        free(stmt);
        return s;
    case STMT_EXPR: {
        s = sprintf_expr(s, stmt->stmt->expr);
        free(stmt->stmt);
        free(stmt);
        return s;
    }
    case STMT_EOF:
        return s;
    default:
        return s;
    }
}

char *tokstr(Token t) {
    switch (t.type) {
    case TOK_EOF:
        return "EOF";
    case TOK_RBRACE:
        return "}";
    case TOK_LBRACE:
        return "{";
    case TOK_RPAREN:
        return "(";
    case TOK_LPAREN:
        return ")";
    case TOK_RBRACKET:
        return "[";
    case TOK_LBRACKET:
        return "]";
    case TOK_NOT:
        return "!";
    case TOK_NEQ:
        return "!=";
    case TOK_EQ:
        return "==";
    case TOK_ASSIGN:
        return "=";
    case TOK_ADD:
        return "+";
    case TOK_ADD_ASSIGN:
        return "+=";
    case TOK_SUB:
        return "-";
    case TOK_SUB_ASSIGN:
        return "-=";
    case TOK_MUL:
        return "*";
    case TOK_MUL_ASSIGN:
        return "*=";
    case TOK_DIV:
        return "/";
    case TOK_DIV_ASSIGN:
        return "/=";
    case TOK_AND:
        return "&&";
    case TOK_OR:
        return "||";
    case TOK_SCOLON:
        return ";";
    case TOK_QMARK:
        return "?";
    case TOK_INC:
        return "++";
    case TOK_DEC:
        return "--";
    case TOK_LT:
        return "<";
    case TOK_GT:
        return ">";
    case TOK_LEQ:
        return "<=";
    case TOK_GEQ:
        return ">=";
    case TOK_IDENT:
        return "";
    case TOK_IF:
        return "if";
    case TOK_ELSE:
        return "else";
    case TOK_VAR:
        return "var";
    case TOK_FUNC:
        return "func";
    case TOK_RETURN:
        return "return";
    case TOK_WHILE:
        return "while";
    case TOK_CONTINUE:
        return "continue";
    case TOK_BREAK:
        return "break";
    case TOK_COLON:
        return ":";
    case TOK_INT:
        return "INT";
    default:
        return "";
    }
}