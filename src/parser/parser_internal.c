/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <libal/pub.h>
#include <libal/dict.h>

#include "../r_internal.h"
#include "parser_internal.h"

void parser_error(int line, int col, char const *msg) {
    r_log("ERR", "bison_flex", line, "Error occurred at column %d: %s\n", col, msg);
}

struct r_logic_sentence* parser_init_bool(R_BOOLEAN v) {
    struct r_logic_sentence *stc = NULL;

    stc = calloc(1, sizeof(*stc));
    CHECK_NOMEM_RT(stc, NULL);

    stc->type = RB_VALUE;
    stc->data.value = v;

    return stc;
}

struct r_logic_sentence* parser_init_variable(const char *name) {
    struct r_logic_sentence *stc = NULL;

    stc = calloc(1, sizeof(*stc));
    CHECK_NOMEM_RT(stc, NULL);

    stc->type = RB_VARIABLE;
    stc->data.name = al_dict_add(name);
    CHECK_NOMEM_DO_RT(stc->data.name, free(stc), NULL);

    return stc;
}

static struct r_logic_sentence* parser_init_two_operands(R_SENTENCE_TYPE type, struct r_logic_sentence *l,
        struct r_logic_sentence *r) {
    struct r_logic_sentence *stc = NULL;

    if (!l || !r) {
        LOG_ERR("Failed to create '%s' sentence due to previous error.", type == RB_AND ? "AND" : "OR");
        r_sentence_destroy(l);
        r_sentence_destroy(r);
        return NULL;
    }

    stc = calloc(1, sizeof(*stc));
    CHECK_NOMEM_RT(stc, NULL);

    stc->type = type;
    stc->data.two[0] = l;
    stc->data.two[1] = r;

    return stc;
}

struct r_logic_sentence* parser_init_and(struct r_logic_sentence *l, struct r_logic_sentence *r) {
    return parser_init_two_operands(RB_AND, l, r);
}

struct r_logic_sentence* parser_init_or(struct r_logic_sentence *l, struct r_logic_sentence *r) {
    return parser_init_two_operands(RB_OR, l, r);
}

struct r_logic_sentence* parser_init_not(struct r_logic_sentence *tgt) {
    struct r_logic_sentence *stc = NULL;

    if (!tgt) {
        LOG_ERR("Failed to create 'NOT' sentence due to previous error.");
        return NULL;
    }

    stc = calloc(1, sizeof(*stc));
    CHECK_NOMEM_RT(stc, NULL);

    stc->type = RB_NOT;
    stc->data.one = tgt;

    return stc;
}
