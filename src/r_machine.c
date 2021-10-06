/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include <r_machine.h>
#include <libal/dict.h>
#include <libal/pub.h>
#include <libal/hash_table.h>

#include "r_internal.h"

#define SENTENCES_ADD_SIZE   4

int r_init() {
    return al_dict_init(100);
}

void r_destroy() {
    al_dict_destroy();
}

int r_sentences_add_exp(struct r_logic_sentences *sts, const char *exp, struct r_error **err) {
    struct r_logic_sentence *st = NULL;

    st = r_sentence_parse(exp, err);
    if (!st) {
        return 1;
    }

    return r_sentences_add(sts, st);
}

int r_sentences_add(struct r_logic_sentences *sts, struct r_logic_sentence *st) {
    void *tmp;

    CHECK_NULLARG_RT(sts, -1);

    if (sts->count == sts->size) {
        tmp = realloc(sts->array, sts->size + SENTENCES_ADD_SIZE);
        CHECK_NOMEM_RT(tmp, -1);

        sts->array = tmp;
        sts->size += SENTENCES_ADD_SIZE;
    }

    sts->array[sts->count] = st;
    ++sts->count;

    return 0;
}

void r_sentences_clean(struct r_logic_sentences *sts) {
    int i;

    for (i = 0; i < sts->count; ++i) {
        r_sentence_destroy(sts->array[i]);
    }

    free(sts->array);

    memset(sts, 0, sizeof(*sts));
}

void r_proposition_clean(struct r_proposition *p) {
    r_sentences_clean(&p->premise);
    r_sentences_clean(&p->conclusion);
}

struct r_logic_sentence* r_convert(const struct r_logic_sentence *st, const struct r_logic_sentence *from,
        const struct r_logic_sentence *to, struct r_error **UNUSED(err)) {
    struct al_hash_table *ht = NULL;
    const struct r_logic_sentence *iter_from, *iter_st, **saved_variable;
    struct r_logic_sentence *dup = NULL, *next_dup, *iter_dup, *tmp;
    int rc;

    CHECK_NULLARG3_RT(st, from, to, NULL);

    ht = al_ht_str_new(sizeof(struct r_logic_sentence*), 0, 0);
    CHECK_NOMEM_RT(ht, NULL);

    for (iter_from = from, iter_st = st; iter_from && iter_st; iter_from = r_dfs_next(from, iter_from), iter_st =
            r_dfs_next(from, iter_st)) {
        if (iter_from->type == RB_VARIABLE) {
            saved_variable = al_ht_getval(ht, &iter_from->data.name);
            if (!saved_variable) {
                rc = al_ht_put(ht, &iter_from->data.name, &iter_st, NULL);
                CHECK_INTERR_GOTO(rc, finish);
            } else {
                CHECK_INTERR_GOTO(!(*saved_variable), finish);
                rc = r_sentence_cmp(*saved_variable, iter_st);
                if (rc) {
                    LOG_WRN("Failed to match sentence and rule.");
                    goto finish;
                }
            }
        }
    }

    dup = r_sentence_dup(to);
    CHECK_NOMEM_GOTO(dup, finish);

    R_SENTENCE_FOR_SAFE(dup, next_dup, iter_dup) {
        if (iter_dup->type == RB_VARIABLE) {
            saved_variable = al_ht_getval(ht, &iter_dup->data.name);
            if (!saved_variable) {
                LOG_WRN("Variable '%s' in sentence 'to' does not be found in sentence 'from'.", iter_dup->data.name);
                r_sentence_destroy(dup);
                dup = NULL;
                goto finish;
            }
            CHECK_INTERR_DO_GOTO(!(*saved_variable), r_sentence_destroy(dup);dup = NULL, finish);

            tmp = r_sentence_dup(*saved_variable);
            CHECK_NOMEM_DO_GOTO(tmp, r_sentence_destroy(dup);dup = NULL, finish);

            *iter_dup = *tmp;
            free(tmp);
        }
    }

    finish:
    al_ht_clear(ht);
    return dup;
}

