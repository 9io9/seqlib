#ifndef _LJW_SEQLIB_LIST_H_

#define _LJW_SEQLIB_LIST_H_

#include "chdrs/fn.h"
#include "chdrs/result.h"
#include "aLocas/include/fsalloc.h"
#include "ec.h"

typedef struct {
    void* next;
    void* prev;
    char d[];
}Item;

typedef struct Lst {
    int esize;
    int size;
    Item* head;
    Item* tail;
    CopyFn fcopy;
    FsAllocator allocator;
}Lst;

typedef struct Vec Vec;
typedef struct Que Que;

// TODO: list interfaces
Error lst_init(Lst* lst, int esize, CopyFn fcopy);
Error lst_appb(Lst* lst, void* data);
Error lst_appf(Lst* lst, void* data);
Error lst_tail(Lst* lst, void* data);
Error lst_head(Lst* lst, void* data);
Error lst2vec(Lst* lst, Vec* vec);
Error lst2que(Lst* lst, Que* que);
Error lst_foreach(Lst* lst, VisFn fvisit);
Error lst_popb(Lst* lst, void* data);
Error lst_popf(Lst* lst, void* data);
Error lst_destroy(Lst* lst);
#endif