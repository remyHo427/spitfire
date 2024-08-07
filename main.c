#include "c.h"

void compile(void);
int dispose(void);

int main(void) {
    compile();
    return dispose();
}

void compile(void) {
    Arena *ast_arena = arena_new();
    arena_free(&ast_arena);
}

int dispose(void) {
    arena_dispose();
#ifndef NDEBUG_MEM
    return dmem_check() > 0;
#else
    return 0;
#endif
}