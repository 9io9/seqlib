#ifndef _LJW_SEQLIB_QUE_H_

#define _LJW_SEQLIB_QUE_H_

#include "vec.h"

typedef struct {
    int begin;
    int end;
    int size;
    int esize;
    int cap;
    void* q;
    CopyFn fcopy;
}Que;

Error que_init(Que* que, int cap, int esize, CopyFn fcopy);
Error que_expand(Que* que);
Error que_enqb(Que* que, void* data);
Error que_enqf(Que* que, void* data);
Error que_deqb(Que* que, void* data);
Error que_deqf(Que* que, void* data);
Error que_front(Que* que, void* data);
Error que_back(Que* que, void* data);
Error que2vec(Que* que, Vec* vec);
Error que_destroy(Que* que);

#endif