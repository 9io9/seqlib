#include <stdlib.h>
#include <string.h>
#include "vec.h"
#include "que.h"
#include "list.h"

Error lst_init(Lst* lst, int esize, CopyFn fcopy) {
    if (lst == NULL || esize <= 0 || fcopy == NULL) {
        return ERROR("lst == NULL or esize <= 0 or fcopy == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    lst->esize = esize;
    lst->size = 0;
    lst->head = lst->tail = NULL;
    lst->fcopy = fcopy;

    return fsalloc_init(&lst->allocator, esize + sizeof(Item), 1, -1);
}

Error lst_appb(Lst* lst, void* data) {
    if (lst == NULL || data == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    Result ni = fsalloc(&lst->allocator);

    if (!ni.has_value) {
        return ni.result.error;
    }

    Item* item = ni.result.data.to_ptr;

    if (lst->size == 0) {
        item->prev = item;
        item->next = NULL;
        lst->head = lst->tail = item;
    } else {
        lst->tail->next = item;
        item->prev = lst->tail;
        item->next = NULL;
        lst->tail = item;
    }

    lst->fcopy(item->d, data);
    lst->size += 1;

    return ERROR(NULL, 0);
}

Error lst_appf(Lst* lst, void* data) {
    if (lst == NULL || data == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    Result ni = fsalloc(&lst->allocator);

    if (!ni.has_value) {
        return ni.result.error;
    }

    Item* item = ni.result.data.to_ptr;

    if (lst->size == 0) {
        item->prev = item;
        item->next = NULL;
        lst->head = lst->tail = item;
    } else {
        lst->head->prev = item;
        item->next = lst->head;
        item->prev = item;
        lst->head = item;
    }

    lst->size += 1;
    lst->fcopy(item->d, data);

    return ERROR(NULL, 0);
}

Error lst_tail(Lst* lst, void* data) {
    if (lst == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    lst->fcopy(data, lst->tail->d);

    return ERROR(NULL, 0);
}

Error lst_head(Lst* lst, void* data) {
    if (lst == NULL || data == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    lst->fcopy(data, lst->head->d);

    return ERROR(NULL, 0);
}

Error lst2vec(Lst* lst, Vec* vec) {
    if (lst == NULL || vec == NULL) {
        return ERROR("lst == NULL or vec == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    Error error = vec_init(vec, lst->size, lst->esize, lst->fcopy);

    if (error.message != NULL) {
        return error;
    }

    Item* v = lst->head;

    while (v != NULL) {
        error = vec_push(vec, v->d);

        if (error.message != NULL) {
            return error;
        }

        v = v->next;
    }

    return ERROR(NULL, 0);
}

Error lst2que(Lst* lst, Que* que) {
    if (lst == NULL || que == NULL) {
        return ERROR("lst == NULL or que == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    Error error = que_init(que, lst->size, lst->esize, lst->fcopy);

    if (error.message != NULL) {
        return error;
    }

    Item* v = lst->head;

    while (v != NULL) {
        error = que_enqb(que, v->d);

        if (error.message != NULL) {
            return error;
        }

        v = v->next;
    }

    return ERROR(NULL, 0);
}

Error lst_foreach(Lst* lst, VisFn fvisit) {
    if (lst == NULL || fvisit == NULL) {
        return ERROR("lst == NULL or fvisit == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR(NULL, 0);
    }

    Item* v = lst->head;

    while (v != NULL) {
        fvisit(v->d);
        v = v->next;
    }

    return ERROR(NULL, 0);
}

Error lst_popb(Lst* lst, void* data) {
    if (lst == NULL || data == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    lst->fcopy(data, lst->tail->d);
    lst->tail = lst->tail->prev;
    lst->size -= 1;

    return fsfree(&lst->allocator, &lst->tail->next);
}

Error lst_popf(Lst* lst, void* data) {
    if (lst == NULL || data == NULL) {
        return ERROR("lst == NULL or data == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    if (lst->size == 0) {
        return ERROR("list is empty", ECODE(SEQLIB, LST, EMPTY));
    }

    lst->fcopy(data, lst->head->d);
    lst->head = lst->head->next;
    
    void* n = lst->head->prev;
    
    lst->head->prev = lst->head;
    lst->size -= 1;

    return fsfree(&lst->allocator, &n);
}

Error lst_destroy(Lst* lst) {
    if (lst == NULL) {
        return ERROR("lst == NULL", ECODE(SEQLIB, LST, ARGV));
    }

    fsalloc_destroy(&lst->allocator);
    memset(lst, 0, sizeof(Lst)); 

    return ERROR(NULL, 0);
}