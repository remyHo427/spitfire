#include "dmem.h"

#define NEW         ((p) = malloc(sizeof *(p)))

typedef struct ptr {
    void *p;
    size_t size;
    struct ptr *next;
} Ptr;

static Ptr *head = NULL;
static size_t memn = 0;
static Ptr *attach(Ptr *, Ptr *);

void *tmalloc(size_t s, const char *file, unsigned long line, const char *f) {
    Ptr *ptr = (malloc)(sizeof (Ptr));

    ptr->p = (malloc)(s);
    ptr->size = s;
    ptr->next = NULL;

#ifndef MEM_DEBUG_QUIET
    printf("%s:%ld %s() allocates %ld...\n", file, line, f, s);
#endif

    head = attach(head, ptr);
    memn += s;

    return ptr->p;
}

static Ptr *attach(Ptr *head, Ptr *ptr) {
    Ptr *p = head;

    if (p == NULL) {
        return ptr;
    }

    while (p->next != NULL) {
        p = p->next;
    }
    p->next = ptr;

    return head;
}
void tfree(void *p, const char *file, unsigned long line, const char *f) {
    Ptr *ptr = head;

    if (p == NULL || ptr == NULL) {
        return;
    }

    while (ptr != NULL) {
        if (ptr->p == p) {
#ifndef MEM_DEBUG_QUIET
            printf("%s:%ld %s() frees %ld bytes...\n", 
                file, line, f, ptr->size);
#endif
            memn -= ptr->size;
            (free)(ptr->p);
            ptr->p = NULL;
            break;
        } else {
            ptr = ptr->next;
        }
    }
}

int dmem_check(void) {
    return memn;
}
