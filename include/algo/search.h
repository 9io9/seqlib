#ifndef _LJW_SEQLIB_ALGO_SEARCH_H_

#define _LJW_SEQLIB_ALGO_SEARCH_H_

#include "chdrs/result.h"
#include "chdrs/fn.h"
#include "ec.h"

typedef struct {
    int s; // size for a element
    void* end; // end of bound
    void* begin; // begin of bound
}range;

Result seq_bsearch(range rng, void* data, CmpFn fcmp);
Error seq_ibsearch(range rng, void* data, CmpFn fcmp, int* index);
Error seq_sort(range rng, CmpFn fcmp, SwapFn fswap);

#endif