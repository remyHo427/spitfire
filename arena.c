#include "c.h"
#define BLOCK_SIZE  10240

static Arena *free_list = NULL;

Arena *pop_free_list(void);

Arena *arena_new(void) {
    Arena *a;

    if ((a = pop_free_list()) != NULL) {
        return a;
    } else {
        a = malloc(sizeof *(a));
        
        a->pool = malloc(BLOCK_SIZE);
        a->curr = 0;
        a->max = BLOCK_SIZE;
        a->next = NULL;

        return a;
    }
}

void arena_free(Arena **ap) {
    Arena *head = *ap;
    Arena *curr;

    if ((curr = free_list) == NULL) {
        free_list = head;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = head;
    }

    *ap = NULL;
}

void arena_dispose(void) {
    Arena *curr, *next;

    curr = free_list;
    while (curr != NULL) {
        next = curr->next;
        free(curr->pool);
        free(curr);
        curr = next;
    }
}

void *arena_alloc(size_t size, Arena *a) {
    Arena *curr, *prev;
    size_t avail;

    assert(size <= BLOCK_SIZE);
    assert(a != NULL);

    prev = curr = a;
    while (curr != NULL) {
        prev = curr;
        
        if (curr->max - curr->curr >= size) {
            avail = curr->curr;
            curr->curr += size;
            return curr->pool + avail;
        }

        curr = curr->next;
    }

    if ((curr = pop_free_list()) != NULL) {
        prev->next = curr;
        avail = curr->curr = 0;
        curr->curr += size;
        
        return curr->pool + avail;
    } else {
        prev->next = curr = arena_new();
        avail = curr->curr;
        curr->curr += size;

        return curr->pool + avail;
    }
}

Arena *pop_free_list(void) {
    Arena *head = free_list;

    if (head == NULL) {
        return NULL;
    } else if (head->next == NULL) {
        free_list = NULL;
        return head;
    } else {
        free_list = head->next;
        head->next = NULL;
        return head;
    }
}