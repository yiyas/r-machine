/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include <stdlib.h>
#include <string.h>

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
    uint32_t i;

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

static int find_variables(const struct r_logic_sentence *st, const struct r_logic_sentence *from,
        struct al_hash_table *ht) {
    const struct r_logic_sentence **saved_variable;
    int rc;

    if (from->type == RB_VARIABLE) {
        saved_variable = al_ht_getval(ht, &from->data.name);
        if (!saved_variable) {
            rc = al_ht_put(ht, &from->data.name, &st, NULL);
            CHECK_INTERR_RT(rc, -1);
        } else {
            CHECK_INTERR_RT(!(*saved_variable), -1);
            rc = r_sentence_cmp(*saved_variable, st);
            if (rc) {
                return 1;
            }
        }
    } else {
        if (st->type != from->type) {
            return 1;
        }

        if (st_type_get_operand_count(from->type) == 2) {
            rc = find_variables(st->data.two[0], from->data.two[0], ht)
                    || find_variables(st->data.two[1], from->data.two[1], ht);
            return rc ? 1 : 0;
        } else if (st_type_get_operand_count(from->type) == 1) {
            rc = find_variables(st->data.one, from->data.one, ht);
            return rc ? 1 : 0;
        } else if (from->type == RB_VALUE) {
            return st->data.value == from->data.value ? 0 : 1;
        } else {
            LOG_UNLIKELY();
            return -1;
        }
    }

    return 0;
}

struct r_logic_sentence* r_convert(const struct r_logic_sentence *st, const struct r_logic_sentence *from,
        const struct r_logic_sentence *to, struct r_error **UNUSED(err)) {
    struct al_hash_table *ht = NULL;
    const struct r_logic_sentence **saved_variable;
    struct r_logic_sentence *dup = NULL, *next_dup, *iter_dup, *tmp;
    int rc;

    CHECK_NULLARG3_RT(st, from, to, NULL);

    ht = al_ht_str_new(sizeof(struct r_logic_sentence*), 0, 0);
    CHECK_NOMEM_RT(ht, NULL);

    rc = find_variables(st, from, ht);
    if (rc) {
        LOG_WRN("Failed to match sentence and rule.");
        goto finish;
    }

    dup = r_sentence_dup(to);
    CHECK_NOMEM_GOTO(dup, finish);

    R_SENTENCE_FOR_SAFE(dup, next_dup, iter_dup)
    {
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
    al_ht_destroy(ht);
    return dup;
}

