#include <stdio.h>
#include <stdlib.h>
#include "arr.h"

static void ferr(Error* e) {
    if (e->message != NULL) {
        fprintf(stderr, "ecode %d: line %u, function %s, file %s for %s\n", e->ec, e->line, e->function, e->file, e->message);
        exit(1);
    }
}

static void fres(Result* r) {
    if (!r->has_value) {
        ferr(&r->result.error);
    }
}

static void i32pvisit(void* d) {
    printf("%d ", *(int*)d);
}

int main(int argc, char* argv[]) {
    Arr arr;
    char* ep;

    int size = strtol(argv[1], &ep, 10);

    int* ds = malloc(sizeof(int) * size);

    for (int i = 2; i < argc; ++i) {
        ds[i - 2] = strtol(argv[i], &ep, 10);
    }

    arr.arr = ds;
    arr.size = size;
    arr.esize = sizeof(int);

    Result result = arr_at(&arr, 0);

    fres(&result);

    int* dr = result.result.data.to_ptr;

    if (dr[0] != ds[0]) {
        fprintf(stderr, "array at returns %d, but expect %d\n", dr[0], ds[0]);
        return 1;
    }

    Error error = arr_foreach(&arr, i32pvisit);

    ferr(&error);

    return 0;
}