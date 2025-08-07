#include <stddef.h>
#include "algo/search.h"

static inline void* at(void* base, int offset, int size) {
    return base + offset * size;
}

static Result pfind(void* base, int l, int r, int es, CmpFn fcmp, SwapFn fswap) {
    int ls, gt, eq;

    for (ls = 0, gt = 0, eq = 0; ls + gt + eq < r - l;) {
        void* pivot = at(base, l + ls, es);
        int nxt = l + 1 + eq + ls;

        switch (fcmp(pivot, at(base, nxt, es))) {
            case left: {
                fswap(pivot, at(base, nxt, es));
                ls += 1;
                break;
            }

            case right: {
                fswap(at(base, nxt, es), at(base, r - gt, es));
                gt += 1;
                break;
            }

            case equal: {
                eq += 1;
                break;
            }

            default: return RESULT_FAIL("invalid return value for compare function", ECODE(SEQLIB, ALGO, IVE));
        }
    }

    return RESULT_SUC(i32, l + ls);
}

static Result asc(void* base, int l, int r, int es, CmpFn fcmp) {
    while (l < r) {
        switch (fcmp(at(base, l, es), at(base, l + 1, es))) {
            case left: {
                return RESULT_SUC(i32, 0);
            }

            case equal:
            case right: {
                l += 1;
                break;
            }

            default: return RESULT_FAIL("invalid return value for compare function", ECODE(SEQLIB, ALGO, IVE));
        }
    }

    return RESULT_SUC(i32, 1);
}

static Error qsort(void* base, int l, int r, int es, CmpFn fcmp, SwapFn fswap) {
    if (l >= r) {
        return ERROR(NULL, 0);
    }

    Result pr = asc(base, l, r, es, fcmp);

    if (!pr.has_value) {
        return pr.result.error;
    }

    if (pr.result.data.to_i32) {
        return ERROR(NULL, 0);
    }

    pr = pfind(base, l, r, es, fcmp, fswap);

    if (!pr.has_value) {
        return pr.result.error;
    }

    Error error = qsort(base, l, pr.result.data.to_i32, es, fcmp, fswap);

    if (error.message != NULL) {
        return error;
    }

    return qsort(base, pr.result.data.to_i32 + 1, r, es, fcmp, fswap);
}

Result seq_bsearch(range rng, void* data, CmpFn fcmp) {
    if (rng.begin == NULL || rng.end == NULL || fcmp == NULL || data == NULL) {
        return RESULT_FAIL("rng.begin == NULL or rng.end == NULL or fcmp == NULL or data == NULL", ECODE(SEQLIB, ALGO, ARGV));
    }

    int l = 0, r = (rng.end - rng.begin) / rng.s;

    while (l <= r) {
        int m = (l + r) >> 1;

        switch (fcmp(at(rng.begin, m, rng.s), data)) {
            case left: {
                r = m - 1;
                break;
            }

            case right: {
                l = m + 1;
                break;
            }

            case equal: {
                return RESULT_SUC(i32, m);
            }

            default: return RESULT_FAIL("invalid return value for compare function", ECODE(SEQLIB, ALGO, IVE));
        }
    }

    return RESULT_FAIL("no data found in range", ECODE(SEQLIB, ALGO, DNF));
}

Error seq_sort(range rng, CmpFn fcmp, SwapFn fswap) {
    if (rng.begin == NULL || rng.end == NULL || fcmp == NULL || fswap == NULL) {
        return ERROR("rng.begin == NULL or rng.end == NULL or fcmp == NULL or fswap == NULL", ECODE(SEQLIB, ALGO, ARGV));
    }
    
    return qsort(rng.begin, 0, (rng.end - rng.begin) / rng.s, rng.s, fcmp, fswap);
}