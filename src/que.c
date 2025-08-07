#include <stdlib.h>
#include <string.h>
#include "cmth/include/calc.h"
#include "que.h"
#include "vec.h"
#include "list.h"

static inline void* at(void* base, int offset, int size) {
    return base + offset * size;
}

Error que_init(Que* que, int cap, int esize, CopyFn fcopy) {
    if (que == NULL || cap <= 0 || esize <= 0 || fcopy == NULL) {
        return ERROR("que == NULL or cap <= 0 or esize <= 0 or fcopy == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    que->begin = que->end = 0;
    que->cap = cap;
    que->size = 0;
    que->esize = esize;
    que->fcopy = fcopy;
    que->q = malloc(cap * esize);

    if (que->q == NULL) {
        return ERROR("malloc returns NULL, lack of memory", ECODE(SEQLIB, QUE, MEM));
    }

    return ERROR(NULL, 0);
}

Error que_expand(Que* que) {
    if (que == NULL) {
        return ERROR("que == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }
    
    int qcap = que->cap << 1;

    void* nq = malloc(qcap * que->esize);

    if (nq == NULL) {
        return ERROR("malloc returns NULL, lack of memory", ECODE(SEQLIB, QUE, MEM));
    }

    for (int i = 1; i <= que->size; ++i) {
        que->fcopy(at(nq, i, que->esize), at(que->q, modi32(que->begin + i, que->cap).result.data.to_i32, que->esize));
    }

    free(que->q);

    que->begin = 0;
    que->end = que->size;
    que->q = nq;
    que->cap = qcap;

    return ERROR(NULL, 0);
}

Error que_enqb(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == que->cap) {
        return ERROR("queue is full", ECODE(SEQLIB, QUE, FULL));
    }

    // sure for no error
    que->end = modi32(que->end + 1, que->cap).result.data.to_i32;
    que->fcopy(at(que->q, que->end, que->esize), data);

    que->size += 1;

    return ERROR(NULL, 0);
}

Error que_enqf(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == que->cap) {
        return ERROR("queue is full", ECODE(SEQLIB, QUE, FULL));
    }

    // sure for no error
    que->fcopy(at(que->q, que->begin, que->esize), data);
    que->begin = mmodi32(que->begin - 1, que->cap).result.data.to_i32;

    que->size += 1;

    return ERROR(NULL, 0);
}

Error que_deqb(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == 0) {
        return ERROR("queue is empty", ECODE(SEQLIB, QUE, EMPTY));
    }

    // sure for no error
    que->fcopy(data, at(que->q, que->end, que->esize));
    que->end = mmodi32(que->end - 1, que->cap).result.data.to_i32;
    que->size -= 1;

    return ERROR(NULL, 0);
}

Error que_deqf(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == 0) {
        return ERROR("queue is empty", ECODE(SEQLIB, QUE, EMPTY));
    }

    // sure for no error
    que->begin = modi32(que->begin + 1, que->cap).result.data.to_i32;
    que->fcopy(data, at(que->q, que->begin, que->esize));
    que->size -= 1;

    return ERROR(NULL, 0);
}

Error que_front(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == 0) {
        return ERROR("queue is empty", ECODE(SEQLIB, QUE, EMPTY));
    }

    // sure for no error
    que->fcopy(data, at(que->q, modi32(que->begin + 1, que->cap).result.data.to_i32, que->esize));

    return ERROR(NULL, 0);
}

Error que_back(Que* que, void* data) {
    if (que == NULL || data == NULL) {
        return ERROR("que == NULL or data == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == 0) {
        return ERROR("queue is empty", ECODE(SEQLIB, QUE, EMPTY));
    }

    // sure for no error
    que->fcopy(data, at(que->q, que->end, que->esize));

    return ERROR(NULL, 0);
}

Error que_foreach(Que* que, VisFn fvisit) {
    if (que == NULL || fvisit == NULL) {
        return ERROR("que == NULL or fvisit == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    for (int i = 0; i < que->size; ++i) {
        fvisit(at(que->q, modi32(que->begin + i + 1, que->cap).result.data.to_i32, que->esize));
    }

    return ERROR(NULL, 0);
}

Error que2vec(Que* que, Vec* vec) {
    if (que == NULL || vec == NULL) {
        return ERROR("que == NULL or vec == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        return ERROR("queue has a NULL pointer", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->fcopy == NULL) {
        return ERROR("queue has a NULL copy function", ECODE(SEQLIB, QUE, INIT));
    }

    if (que->size == 0) {
        return ERROR("queue is empty", ECODE(SEQLIB, QUE, EMPTY));
    }

    Error error = vec_init(vec, que->size, que->esize, que->fcopy);

    if (error.message != NULL) {
        return error;
    }

    for (int i = 0; i < que->size; ++i) {
        // sure for no error
        que->fcopy(at(vec->v, i, que->esize), at(que->q, modi32(que->begin + i + 1, que->cap).result.data.to_i32, que->esize));
    }

    vec->size = que->size;

    return ERROR(NULL, 0);
}

Error que_destroy(Que* que) {
    if (que == NULL) {
        return ERROR("que == NULL", ECODE(SEQLIB, QUE, ARGV));
    }

    if (que->q == NULL) {
        memset(que, 0, sizeof(Que));
        return ERROR(NULL, ECODE(SEQLIB, QUE, INIT));
    }

    free(que->q);
    memset(que, 0, sizeof(Que));

    return ERROR(NULL, 0);
}