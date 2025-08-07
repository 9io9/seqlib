#include <stdlib.h>
#include <string.h>
#include "vec.h"
#include "que.h"
#include "list.h"

#define DEFAULT_CAP 16

static inline void* at(void* base, int offset, int size) {
    return base + offset * size;
}

static Error expand(Vec* v) {
    void* nv = malloc((v->cap << 1) * v->esize);

    if (nv == NULL) {
        return ERROR("malloc return NULL, lack of memory", ECODE(SEQLIB, VEC, MEM));
    }

    memcpy(nv, v->v, v->size * v->esize);

    free(v->v);

    v->cap <<= 1;
    v->v = nv;

    return ERROR(NULL, 0);
}

Error vec_init(Vec* v, int cap, int esize, CopyFn fcopy) {
    if (v == NULL || esize <= 0 || fcopy == NULL) {
        return ERROR("v == NULL or esize <= 0 or fcopy == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (cap <= 0) {
        cap = DEFAULT_CAP;
    }

    v->cap = cap;
    v->esize = esize;
    v->size = 0;
    v->fcopy = fcopy;
    v->v = malloc(esize * cap);

    return ERROR(NULL, 0);
}

Error vec_push(Vec* v, void* data) {
    if (v == NULL || data == NULL) {
        return ERROR("v == NULL or data == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->fcopy == NULL) {
        return ERROR("vector has a NULL copy function", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->size == v->cap) {
        Error error = expand(v);
        
        if (error.message != NULL) {
            return error;
        }
    }

    v->fcopy(at(v->v, v->size, v->esize), data);

    v->size += 1;

    return ERROR(NULL, 0);
}

Error vec_pop(Vec* v, void* data) {
    if (v == NULL || data == NULL) {
        return ERROR("v == NULL or data == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->fcopy == NULL) {
        return ERROR("vector has a NULL copy function", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->size == 0) {
        return ERROR("vector is empty", ECODE(SEQLIB, VEC, EMPTY));
    }

    v->size -= 1;

    v->fcopy(data, at(v->v, v->size, v->esize));

    return ERROR(NULL, 0);
}

Error vec_at(Vec* v, int i, void* data) {
    if (v == NULL || i < 0 || data == NULL) {
        return ERROR("v == NULL or i < 0 or data == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->fcopy == NULL) {
        return ERROR("vector has a NULL copy function", ECODE(SEQLIB, VEC, INIT));
    }

    if (i >= v->size) {
        return ERROR("index out of bound for vector", ECODE(SEQLIB, VEC, OBD));
    }

    v->fcopy(data, at(v->v, i, v->esize));

    return ERROR(NULL, 0);
}

Error vec2rng(Vec* v, int i, int len, range* rng) {
    if (v == NULL || i < 0 || len <= 0 || rng == NULL) {
        return ERROR("v == NULL or i < 0 or len <= 0 or rng == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (i >= v->size || i + len > v->size) {
        return ERROR("index out of bound for vector", ECODE(SEQLIB, VEC, OBD));
    }

    rng->begin = at(v->v, i, v->esize);
    rng->end = at(v->v, i + len - 1, v->esize);
    rng->s = v->esize;

    return ERROR(NULL, 0);
}

Error vec_insert(Vec* v, int i, void* data) {
    if (v == NULL || i < -1 || data == NULL) {
        return ERROR("v == NULL or i < -1 or data == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (i >= v->size - 1) {
        return ERROR("index out of bound for vector", ECODE(SEQLIB, VEC, OBD));
    }

    if (v->fcopy == NULL) {
        return ERROR("vector has a NULL copy function", ECODE(SEQLIB, VEC, INIT));
    }

    if (v->size == v->cap) {
        Error error = expand(v);

        if (error.message != NULL) {
            return error;
        }
    }

    memmove(at(v->v, i + 2, v->esize), at(v->v, i + 1, v->esize), (v->size - i - 1) * v->esize);

    v->fcopy(at(v->v, i + 1, v->esize), data);

    v->size += 1;

    return ERROR(NULL, 0);
}

Error vec_remove(Vec* v, int i, void* data) {
    if (v == NULL || i < 0 || data == NULL) {
        return ERROR("v == NULL or i < 0 or data == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    if (i >= v->size - 1) {
        return ERROR("index out of bound for vector", ECODE(SEQLIB, VEC, OBD));
    }

    if (v->fcopy == NULL) {
        return ERROR("vector has a NULL copy function", ECODE(SEQLIB, VEC, INIT));
    }

    v->fcopy(data, at(v->v, i, v->esize));

    memcpy(at(v->v, i, v->esize), at(v->v, i + 1, v->esize), (v->size - i - 1) * v->esize);

    v->size -= 1;

    return ERROR(NULL, 0);
}

Error vec_foreach(Vec* v, VisFn fvisit) {
    if (v == NULL || fvisit == NULL) {
        return ERROR("v == NULL or fvisit == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        return ERROR("vector has a NULL pointer", ECODE(SEQLIB, VEC, INIT));
    }

    for (int i = 0; i < v->size; ++i) {
        fvisit(at(v->v, i, v->esize));
    }

    return ERROR(NULL, 0);
}

Error vec_destroy(Vec* v) {
    if (v == NULL) {
        return ERROR("v == NULL", ECODE(SEQLIB, VEC, ARGV));
    }

    if (v->v == NULL) {
        memset(v, 0, sizeof(Vec));
        return ERROR(NULL, 0);
    }

    free(v->v);
    memset(v, 0, sizeof(Vec));

    return ERROR(NULL, 0);
}