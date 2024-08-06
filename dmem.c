#include "c.h"

typedef struct ptr {
    void *p;
    size_t size;
    struct ptr *next;
    const char *file;
    unsigned long line;
    const char *func;
} Ptr;

static Ptr *head = NULL;
static size_t memn = 0;
static size_t debug_dmem_mem = 0;
static Ptr *attach(Ptr *, Ptr *);

void *tmalloc(size_t s, const char *file, unsigned long line, const char *f) {
    Ptr *ptr = (malloc)(sizeof (Ptr));
    debug_dmem_mem += sizeof (Ptr);
    
    ptr->p = (malloc)(s);
    ptr->size = s;
    ptr->next = NULL;
    ptr->file = file;
    ptr->func = f;
    ptr->line = line;

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
    Ptr *curr = head;
    Ptr *next;

    while (curr != NULL) {
        debug_dmem_mem -= sizeof *(curr);
        next = curr->next;
        if (curr->p != NULL) {
            printf("non-NULL pointer allocated in function %s() at %s:%ld\n",
                curr->func, curr->file, curr->line);
        }
        (free)(curr);
        curr = next;
    }

    assert(debug_dmem_mem == 0);
    return memn;
}
