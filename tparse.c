#include "c.h"

int main(void) {
    parse_init(";");
    parse();

    dmem_check();
    return 0;
}
