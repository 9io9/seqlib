#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"
#include "algo/search.h"

static void i32copy(void* d, void* s) {
    *(int*)d = *(int*)s;
}

static void ferr(Error* e) {
    if (e->message != NULL) {
        fprintf(stderr, "line %u, function %s, file %s for %s\n", e->line, e->function, e->file, e->message);
        exit(1);
    }
}

static void fres(Result* r) {
    if (!r->has_value) {
        ferr(&r->result.error);
    }
}

static order i32cmp(void* a, void* b) {
    int* ia = a, *ib = b;

    return *ia > *ib ? left : *ia == *ib ? equal : right;
}

static void i32swap(void* a, void* b) {
    int s;
    int* ia = a, *ib = b;

    s = *ia;
    *ia = *ib;
    *ib = s;
}

static void i32pvisit(void* d) {
    printf("%d ", *(int*)d);
}

int main(int argc, char* argv[]) {
    Vec v;

    Error error = vec_init(&v, 10, sizeof(int), i32copy);

    ferr(&error);

    char* ep;

    for (int i = 2; i < argc; ++i) {
        int d = strtol(argv[i], &ep, 10);
        
        error = vec_push(&v, &d);

        ferr(&error);
    }

    range rng;
        
    error = vec2rng(&v, 0, v.size, &rng);

    ferr(&error);

    if (strcmp("sort", argv[1]) == 0) {
        error = seq_sort(rng, i32cmp, i32swap);

        ferr(&error);
    
        error = vec_foreach(&v, i32pvisit);

        printf("\n");
    } else if (strcmp("search", argv[1]) == 0) {
        int sd = 0;

        error = vec_at(&v, v.size >> 1, &sd);

        ferr(&error);

        Result result = seq_bsearch(rng, &sd, i32cmp);

        fres(&result);

        if (result.result.data.to_i32 != v.size >> 1) {
            fprintf(stderr, "bsearch has value %d, but expect %d\n", result.result.data.to_i32, v.size >> 1);
            return 1;
        }
    } else {
        fprintf(stderr, "invalid test command for %s\n", argv[1]);
        return 1;
    }

    return 0;
}