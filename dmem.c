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
static size_t debug_list_len = 0;
static Ptr *attach(Ptr *, Ptr *);

// need to tell GCC to not raise unused parameter errors
// when MEM_DEBUG_QUIET macro is defined
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void *tmalloc(
        size_t s, 
        const char *file, 
        unsigned long line, 
        const char *f
    ) {
#pragma GCC diagnostic pop

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

    debug_list_len++;
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

// ditto
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void tfree(
        void *p, 
        const char *file, 
        unsigned long line, 
        const char *f
    ) {
#pragma GCC diagnostic pop

    Ptr *curr = head;
    Ptr *prev = NULL;

    if (p == NULL) {
        fprintf(stderr, "tfree: freeing null pointer at %s:%ld %s()\n",
            file, line, f);
        exit(1);
    }

    while (curr != NULL) {
        if (curr->p == p) {

#ifndef MEM_DEBUG_QUIET
            printf("%s:%ld %s() frees %ld bytes...\n", 
                file, line, f, curr->size);
#endif

            memn -= curr->size;
            (free)(curr->p);
            curr->p = NULL;

            if (prev != NULL) {
                prev->next = curr->next;
                free(curr);
                curr = NULL;
                debug_dmem_mem -= sizeof (Ptr);
                debug_list_len--;
            }

            break;
        } else {
            prev = curr;
            curr = curr->next;
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
            printf("non-NULL pointer allocated in %s() at %s:%ld\n",
                curr->func, curr->file, curr->line);
        }
        debug_list_len--;
        (free)(curr);
        curr = next;
    }

    assert(debug_dmem_mem == 0);
    assert(debug_list_len == 0);
    return memn;
}
