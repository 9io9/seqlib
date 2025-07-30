#include <stdio.h>
#include <stdlib.h>
#include "que.h"

static void ferr(Error* e) {
    if (e->message != NULL) {
        fprintf(stderr, "line %u, function %s, file %s for %s\n", e->line, e->function, e->file, e->message);
        exit(1);
    }
}

static void i32copy(void* d, void* s) {
    *(int*)d = *(int*)s;
}

int main(int argc, char* argv[]) {
    char* ep;
    
    int cap = strtol(argv[1], &ep, 10);

    Que que;
    Error error;

    error = que_init(&que, cap, sizeof(int), i32copy);

    ferr(&error);

    for (int i = 0; i < cap; ++i) {
        int d = strtol(argv[i + 2], &ep, 10);

        error = que_enqb(&que, &d);

        ferr(&error);
    }

    int dl = strtol(argv[argc - 1], &ep, 10);

    error = que_enqb(&que, &dl);

    if (error.message == NULL) {
        fprintf(stderr, "enqueue back should return queue is full, but not\n");
        return 1;
    }

    for (int i = 0; i < cap; ++i) {
        int d = 0;
        int td = strtol(argv[i + 2], &ep, 10);

        error = que_deqf(&que, &d);

        ferr(&error);

        if (d != td) {
            fprintf(stderr, "dequeue front returns %d, but expect %d\n", d, td);
        }
    }

    error = que_deqf(&que, &dl);

    if (error.message == NULL) {
        fprintf(stderr, "dequeue front should return queue is empty, but not\n");
        return 1;
    }

    error = que_destroy(&que);

    ferr(&error);

    return 0;
}