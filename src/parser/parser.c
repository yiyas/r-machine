/**
 * Copyright (c) 2015 - 2017 YIYAS
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

#include "parser.h"

void parser_error(int line, int col, char const *msg) {
    printf("Error occurred at %d-%d: %s\n", line, col, msg);
}

void parser_init_bool(struct r_logic_sentence *expr, R_BOOLEAN v) {
    expr->type = RB_VALUE;
    expr->data.value = v;
}

int parser_init_variable(struct r_logic_sentence *expr, const char *name) {
    expr->type = RB_VARIABLE;
    expr->data.name = al_dict_add(name);
    return expr->data.name ? 0 : 1;
}

int parser_init_and(struct r_logic_sentence *expr, const struct r_logic_sentence *l,
        struct r_logic_sentence *r) {
    expr->type = RB_AND;
    expr->data.two[0] = calloc(1, sizeof(*l));
    CHECK_NOMEM_RT(expr->data.two[0], 1);

    expr->data.two[1] = calloc(1, sizeof(*r));
    CHECK_NOMEM_DO_RT(expr->data.two[1], free(expr->data.two[0]), 1);

    *(expr->data.two[0]) = *l;
    *(expr->data.two[1]) = *r;

    return 0;
}

int parser_init_or(struct r_logic_sentence *expr, const struct r_logic_sentence *l,
        struct r_logic_sentence *r) {
    expr->type = RB_OR;
    expr->data.two[0] = calloc(1, sizeof(*l));
    CHECK_NOMEM_RT(expr->data.two[0], 1);

    expr->data.two[1] = calloc(1, sizeof(*r));
    CHECK_NOMEM_DO_RT(expr->data.two[1], free(expr->data.two[0]), 1);

    *(expr->data.two[0]) = *l;
    *(expr->data.two[1]) = *r;

    return 0;
}

int parser_init_not(struct r_logic_sentence *expr, const struct r_logic_sentence *tgt) {
    expr->type = RB_NOT;
    expr->data.one = calloc(1, sizeof(*tgt));
    CHECK_NOMEM_RT(expr->data.one, 1);

    *(expr->data.one) = *tgt;

    return 0;
}
