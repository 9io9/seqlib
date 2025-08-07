#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static void i32copy(void* d, void* s) {
    *(int*)d = *(int*)s;
}

static void ferr(Error* e) {
    if (e->message != NULL) {
        fprintf(stderr, "code %d: line %u, function %s, file %s for %s\n", e->ec, e->line, e->function, e->file, e->message);
        exit(1);
    }
}

static void i32pvisit(void* d) {
    printf("%d ", *(int*)d);
}

int main(int argc, char* argv[]) {
    Lst lst;
    char* ep;

    Error error = lst_init(&lst, sizeof(int), i32copy);

    ferr(&error);

    for (int i = 1; i < argc; ++i) {
        int d = strtol(argv[i], &ep, 10);

        error = lst_appb(&lst, &d);

        ferr(&error);
    }

    error = lst_foreach(&lst, i32pvisit);

    ferr(&error);

    int d = strtol(argv[argc - 1], &ep, 10);
    int ld = 0;

    error = lst_tail(&lst, &ld);

    ferr(&error);

    if (ld != d) {
        fprintf(stderr, "list tail returns %d, but expect %d\n", ld, d);
        return 1;
    }

    error = lst_popb(&lst, &d);

    ferr(&error);

    if (d != ld) {
        fprintf(stderr, "list popb returns %d, but expect %d\n", d, ld);
        return 1;
    }
}