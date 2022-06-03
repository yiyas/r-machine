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
#include <libal/dict.h>

#include "../r_internal.h"
#include "parser_internal.h"

void rp_error(struct rp_param *param, int line, int col, char const *msg) {
    param->err.line = line;
    param->err.col = col;
    param->err.msg = al_dict_add(msg);
}

struct r_expression* rp_new_expr_def(struct r_definition *def) {
    struct r_expression *expr = NULL;

    expr = calloc(1, sizeof(*expr));
    CHECK_NOMEM_RT(expr, NULL);

    expr->type = R_DEF;
    expr->data.def =def;

    return expr;
}

struct r_expression* rp_new_expr_stmt(struct r_statment *stmt) {
    struct r_expression *expr = NULL;

    expr = calloc(1, sizeof(*expr));
    CHECK_NOMEM_RT(expr, NULL);

    expr->type = R_STMT;
    expr->data.stmt =stmt;

    return expr;
}

struct r_definition* rp_new_def(const char *name) {
    struct r_definition *def = NULL;

    def = calloc(1, sizeof(*def));
    CHECK_NOMEM_RT(def, NULL);

    def->name = al_dict_add(name);

    return def;
}

struct r_definition* rp_new_def2(const char *name, const char *UNUSED(type)) {
    return rp_new_def(name);
}

struct r_definition* rp_new_def3(const char *name, const char *UNUSED(from), const char *UNUSED(to)) {
    return rp_new_def(name);
}

struct r_statment* rp_new_stmt(const char *name, const char *UNUSED(name2)) {
    struct r_statment *stmt = NULL;

    stmt = calloc(1, sizeof(*stmt));
    CHECK_NOMEM_RT(stmt, NULL);

    stmt->name = al_dict_add(name);

    return stmt;
}
