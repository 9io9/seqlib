#include <stddef.h>
#include "arr.h"

static inline void* at(void* base, int offset, int size) {
    return base + offset * size;
}

Error arr2rng(Arr* arr, int i, int len, range* rng) {
    if (arr == NULL || i < 0 || len <= 0 || rng == NULL) {
        return ERROR("arr == NULL or i < 0 or len <= 0 or rng == NULL", ECODE(SEQLIB, ARR, ARGV));
    }

    if (i + len - 1 >= arr->size) {
        return ERROR("out of bound for array", ECODE(SEQLIB, ARR, OBD));
    }

    rng->begin = at(arr->arr, i, arr->esize);
    rng->end = at(arr->arr, i + len - 1, arr->esize);
    rng->s = arr->esize;

    return ERROR(NULL, 0);
}

Result arr_at(Arr* arr, int i) {
    if (arr == NULL || i < 0) {
        return RESULT_FAIL("arr == NULL or i < 0", ECODE(SEQLIB, ARR, ARGV));
    }

    if (i >= arr->size) {
        return RESULT_FAIL("out of bound for array", ECODE(SEQLIB, ARR, OBD));
    }

    return RESULT_SUC(ptr, at(arr->arr, i, arr->esize));
}

Error arr_foreach(Arr* arr, VisFn fvisit) {
    if (arr == NULL || fvisit == NULL) {
        return ERROR("arr == NULL or fvisit == NULL", ECODE(SEQLIB, ARR, ARGV));
    }

    for (int i = 0; i < arr->size; ++i) {
        fvisit(at(arr->arr, i, arr->esize));
    }

    return ERROR(NULL, 0);
}