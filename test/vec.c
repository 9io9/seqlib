#include <stdlib.h>
#include <stdio.h>
#include "vec.h"

static void i32copy(void* d, void* s) {
    *(int*)d = *(int*)s;
}

static void i32pvisit(void* d) {
    printf("%d ", *(int*)d);
}

static void ferr(Error* error) {
    if (error->message != NULL) {
        fprintf(stderr, "line %u, function %s, file %s for %s\n", error->line, error->function, error->file, error->message);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    char* ep;

    int cap = strtol(argv[1], &ep, 10);

    Vec v;

    Error error = vec_init(&v, cap, sizeof(int), i32copy);

    ferr(&error);

    for (int i = 2; i < argc; ++i) {
        int d = strtol(argv[i], &ep, 10);

        error = vec_push(&v, &d);

        ferr(&error);
    }

    error = vec_foreach(&v, i32pvisit);

    ferr(&error);

    for (int i = 0; i < argc - 2; ++i) {
        int d = 0, td = strtol(argv[i + 2], &ep, 10);

        error = vec_at(&v, i, &d);

        ferr(&error);

        if (d != td) {
            fprintf(stderr, "vector push has value %d, but expect %d\n", d, td);
            return 1;
        }
    }

    int pd = 0, tpd = strtol(argv[argc - 1], &ep, 10);

    error = vec_pop(&v, &pd);

    ferr(&error);

    if (pd != tpd) {
        fprintf(stderr, "vector pop has value %d, but expect %d\n", pd, tpd);
        return 1;
    }

    int itd = 53, id = 0, pid = 0;

    error = vec_at(&v, 0, &pid);

    ferr(&error);

    error = vec_insert(&v, -1, &itd);

    ferr(&error);

    error = vec_at(&v, 0, &id);

    ferr(&error);

    if (id != itd) {
        fprintf(stderr, "vector insert has value %d at index 0, but expect %d\n", id, itd);
        return 1;
    }

    error = vec_remove(&v, 0, &id);

    ferr(&error);

    if (id != itd) {
        fprintf(stderr, "vector remove has value %d at index 0, but expect %d\n", id, itd);
        return 1;
    }

    error = vec_at(&v, 0, &id);

    ferr(&error);

    if (id != pid) {
        fprintf(stderr, "vector remove has value %d at previous index 0, but expect %d\n", id, pid);
        return 1;
    }

    error = vec_destroy(&v);

    ferr(&error);

    return 0;
}