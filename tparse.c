#include "c.h"

char *print_ast(char *s);

int main(void) {
    print_ast("1 + 1;");

    dmem_check();
    return 0;
}

char *print_ast(char *s) {
    parse_init(s);
    parse();


    return "";
}