#include "c.h"

int main(void) {
    parse_init("1 + 1;");
    parse();

    dmem_check();
    return 0;
}
