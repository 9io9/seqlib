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

static void i32pvisit(void* d) {
    printf("%d ", *(int*)d);
}

int main(int argc, char* argv[]) {
    char* ep;
    
    int cap = strtol(argv[1], &ep, 10);

    Que que;
    Error error;

    // init test
    error = que_init(&que, cap, sizeof(int), i32copy);

    ferr(&error);

    // enqb test
    for (int i = 0; i < cap; ++i) {
        int d = strtol(argv[i + 2], &ep, 10);

        error = que_enqb(&que, &d);

        ferr(&error);
    }

    // for each test
    error = que_foreach(&que, i32pvisit);

    ferr(&error);

    printf("\n");

    int tdl = strtol(argv[argc - 1], &ep, 10);

    error = que_enqb(&que, &tdl);

    // full queue test
    if (error.message == NULL) {
        fprintf(stderr, "enqueue back should return queue is full, but not\n");
        return 1;
    }

    // expand test
    error = que_expand(&que);

    ferr(&error);

    error = que_enqb(&que, &tdl);

    ferr(&error);

    error = que_foreach(&que, i32pvisit);

    ferr(&error);

    printf("\n");

    int dl = 0;

    error = que_deqb(&que, &dl);

    ferr(&error);

    // deqb test
    if (dl != tdl) {
        fprintf(stderr, "dequeue back returns %d, but expect %d\n", dl, tdl);
        return 1;
    }

    // deqf test
    for (int i = 0; i < cap; ++i) {
        int d = 0;
        int td = strtol(argv[i + 2], &ep, 10);

        error = que_deqf(&que, &d);

        ferr(&error);

        if (d != td) {
            fprintf(stderr, "dequeue front returns %d, but expect %d\n", d, td);
            return 1;
        }
    }

    error = que_deqf(&que, &dl);

    // empty queue test
    if (error.message == NULL) {
        fprintf(stderr, "dequeue front should return queue is empty, but not\n");
        return 1;
    }

    // destroy test
    error = que_destroy(&que);

    ferr(&error);

    return 0;
}