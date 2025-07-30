#include <stdlib.h>
#include <string.h>
#include "cmth/include/calc.h"
#include "que.h"

static inline void* at(void* base, int offset, int size) {
    return base + offset * size;
}

Error que_init(Que* que, int cap, int esize, CopyFn fcopy) {
    if (que == NULL || cap <= 0 || esize <= 0 || fcopy == NULL) {
        return ERROR("que == NULL or cap <= 0 or esize <= 0 or fcopy == NULL");
    }

    que->begin = que->end = 0;
    que->cap = cap;
    que->size = 0;
    que->esize = esize;
    que->fcopy = fcopy;
    que->q = malloc(cap * esize);

    if (que->q == NULL) {
        return ERROR("malloc returns NULL, lack of memory");
    }

    return ERROR(NULL);
}

Error que_expand(Que* que) {
    if (que == NULL) {
        return ERROR("que == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }
    
    que->cap <<= 1;

    void* nq = malloc(que->cap * que->esize);

    if (nq == NULL) {
        que->cap >>= 1;
        return ERROR("malloc returns NULL, lack of memory");
    }

    for (int i = 0; i < que->size; ++i) {
        que->fcopy(at(nq, i, que->esize), at(que->q, que->begin + i, que->esize));
    }

    free(que->q);

    que->begin = 0;
    que->end = que->size - 1;
    que->q = nq;

    return ERROR(NULL);
}

Error que_enqb(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == que->cap) {
        return ERROR("queue is full");
    }

    // sure for no error
    que->end = modi32(que->end + 1, que->cap).result.data.to_i32;
    que->fcopy(at(que->q, que->end, que->esize), data);

    que->size += 1;

    return ERROR(NULL);
}

Error que_enqf(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == que->cap) {
        return ERROR("queue is full");
    }

    // sure for no error
    que->fcopy(at(que->q, que->begin, que->esize), data);
    que->begin = mmodi32(que->begin - 1, que->cap).result.data.to_i32;

    que->size += 1;

    return ERROR(NULL);
}

Error que_deqb(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == 0) {
        return ERROR("queue is empty");
    }

    // sure for no error
    que->fcopy(data, at(que->q, que->end, que->esize));
    que->end = mmodi32(que->end - 1, que->cap).result.data.to_i32;
    que->size -= 1;

    return ERROR(NULL);
}

Error que_deqf(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == 0) {
        return ERROR("queue is empty");
    }

    // sure for no error
    que->begin = modi32(que->begin + 1, que->cap).result.data.to_i32;
    que->fcopy(data, at(que->q, que->begin, que->esize));
    que->size -= 1;

    return ERROR(NULL);
}

Error que_front(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == 0) {
        return ERROR("queue is empty");
    }

    // sure for no error
    que->fcopy(data, at(que->q, modi32(que->begin + 1, que->cap).result.data.to_i32, que->esize));

    return ERROR(NULL);
}

Error que_back(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == 0) {
        return ERROR("queue is empty");
    }

    // sure for no error
    que->fcopy(data, at(que->q, que->end, que->esize));

    return ERROR(NULL);
}

Error que2vec(Que* que, Vec* vec) {
    if (que == NULL || vec == NULL) {
        return ERROR("que == NULL or vec == NULL");
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer");
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function");
    }

    if (que->size == 0) {
        return ERROR("queue is empty");
    }

    Error error = vec_init(vec, que->size, que->esize, que->fcopy);

    if (error.message != NULL) {
        return error;
    }

    for (int i = 0; i < que->size; ++i) {
        que->fcopy(at(vec->v, i, que->esize), at(que->q, que->end, que->esize));
        // sure for no error
        que->end = mmodi32(que->end - 1, que->cap).result.data.to_i32;
    }

    vec->size = que->size;

    return ERROR(NULL);
}

Error que_destroy(Que* que) {
    if (que == NULL) {
        return ERROR("que == NULL");
    }

    if (que->q == NULL) {
        memset(que, 0, sizeof(Que));
        return ERROR(NULL);
    }

    free(que->q);
    memset(que, 0, sizeof(Que));

    return ERROR(NULL);
}