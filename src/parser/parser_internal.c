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

#include "parser_internal.h"

void parser_error(int line, int col, char const *msg) {
    printf("Error occurred at %d-%d: %s\n", line, col, msg);
}

void parser_init_bool(struct r_logic_sentence *stc, R_BOOLEAN v) {
    stc->type = RB_VALUE;
    stc->data.value = v;
}

int parser_init_variable(struct r_logic_sentence *stc, const char *name) {
    stc->type = RB_VARIABLE;
    stc->data.name = al_dict_add(name);
    return stc->data.name ? 0 : 1;
}

int parser_init_and(struct r_logic_sentence *stc, const struct r_logic_sentence *l,
        struct r_logic_sentence *r) {
    stc->type = RB_AND;
    stc->data.two[0] = calloc(1, sizeof(*l));
    CHECK_NOMEM_RT(stc->data.two[0], 1);

    stc->data.two[1] = calloc(1, sizeof(*r));
    CHECK_NOMEM_DO_RT(stc->data.two[1], free(stc->data.two[0]), 1);

    *(stc->data.two[0]) = *l;
    *(stc->data.two[1]) = *r;

    return 0;
}

int parser_init_or(struct r_logic_sentence *stc, const struct r_logic_sentence *l,
        struct r_logic_sentence *r) {
    stc->type = RB_OR;
    stc->data.two[0] = calloc(1, sizeof(*l));
    CHECK_NOMEM_RT(stc->data.two[0], 1);

    stc->data.two[1] = calloc(1, sizeof(*r));
    CHECK_NOMEM_DO_RT(stc->data.two[1], free(stc->data.two[0]), 1);

    *(stc->data.two[0]) = *l;
    *(stc->data.two[1]) = *r;

    return 0;
}

int parser_init_not(struct r_logic_sentence *stc, const struct r_logic_sentence *tgt) {
    stc->type = RB_NOT;
    stc->data.one = calloc(1, sizeof(*tgt));
    CHECK_NOMEM_RT(stc->data.one, 1);

    *(stc->data.one) = *tgt;

    return 0;
}
