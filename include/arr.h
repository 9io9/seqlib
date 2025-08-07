/*
    in the past, you need to pass two arguments to represent an array,
    but not you only need a Arr struct, and you can convert it to range so that you can apply algorithms of seqlib on it (like sort and bsearch ...)
*/

#ifndef _LJW_SEQLIB_ARR_H_

#define _LJW_SEQLIB_ARR_H_

#include "algo/search.h"

typedef struct Arr {
    int size;
    int esize;
    void* arr;
}Arr;

Error arr2rng(Arr* arr, int i, int len, range* rng);
Result arr_at(Arr* arr, int i);
Error arr_foreach(Arr* arr, VisFn fvisit);

#endif