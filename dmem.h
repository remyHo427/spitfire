#ifndef SPITFIRE_DMEM_H

#include <stdio.h>
#include <stdlib.h>

#define malloc(s)   (tmalloc(s, __FILE__, __LINE__, __func__))
#define free(p)     (tfree(p, __FILE__, __LINE__, __func__))

void *tmalloc(size_t s, const char *, unsigned long, const char *);
void  tfree(void *, const char *, unsigned long, const char *);

int dmem_check(void);

#endif