#ifndef _LJW_SEQLIB_VEC_H_

#define _LJW_SEQLIB_VEC_H_

#include "chdrs/result.h"
#include "chdrs/fn.h"
#include "algo/search.h"

typedef struct Vec {
    int cap;
    int size;
    int esize;
    CopyFn fcopy;
    void* v;
}Vec;

typedef struct Que Que;
typedef struct Lst Lst;

// TODO: convertion of vec
Error vec_init(Vec* v, int cap, int esize, CopyFn fcopy);
Error vec_push(Vec* v, void* data);
Error vec_pop(Vec* v, void* data);
Error vec_at(Vec* v, int i, void* data);
Error vec2rng(Vec* v, int i, int len, range* rng);
Error vec2que(Vec* v, Que* q);
Error vec2lst(Vec* v, Lst* l);
Error vec_insert(Vec* v, int i, void* data);
Error vec_remove(Vec* v, int i, void* data);
Error vec_foreach(Vec* v, VisFn fvisit);
Error vec_destroy(Vec* v);

#endif