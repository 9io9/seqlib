#ifndef _LJW_SEQLIB_QUE_H_

#define _LJW_SEQLIB_QUE_H_

#include "chdrs/fn.h"
#include "chdrs/result.h"
#include "ec.h"

typedef struct Que {
    int begin;
    int end;
    int size;
    int esize;
    int cap;
    void* q;
    CopyFn fcopy;
}Que;

typedef struct Vec Vec;
typedef struct Lst Lst;

//TODO: convertion of queue
Error que_init(Que* que, int cap, int esize, CopyFn fcopy);
Error que_expand(Que* que);
Error que_enqb(Que* que, void* data);
Error que_enqf(Que* que, void* data);
Error que_deqb(Que* que, void* data);
Error que_deqf(Que* que, void* data);
Error que_front(Que* que, void* data);
Error que_back(Que* que, void* data);
Error que_foreach(Que* que, VisFn fvisit);
Error que2vec(Que* que, Vec* vec);
Error que2lst(Que* que, Lst* lst);
Error que_destroy(Que* que);

#endif