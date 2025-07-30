#ifndef _LJW_SEQLIB_VEC_H_

#define _LJW_SEQLIB_VEC_H_

#include "chdrs/result.h"
#include "chdrs/fn.h"
#include "algo/search.h"

typedef struct {
    int cap;
    int size;
    int esize;
    CopyFn fcopy;
    void* v;
}Vec;

Error vec_init(Vec* v, int cap, int esize, CopyFn fcopy);
Error vec_push(Vec* v, void* data);
Error vec_pop(Vec* v, void* data);
Error vec_at(Vec* v, int i, void* data);
Error vec2rng(Vec* v, int i, int len, range* rng);
Error vec_insert(Vec* v, int i, void* data);
Error vec_remove(Vec* v, int i, void* data);
Error vec_foreach(Vec* v, VisFn fvisit);
Error vec_destroy(Vec* v);

#endif