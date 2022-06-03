/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef PARSER_INTERNAL_H_
#define PARSER_INTERNAL_H_

#include <r_machine.h>

struct rp_error {
    int line;
    int col;
    const char *msg;
};

struct rp_param {
    void *p;
    struct r_expression *expr;

    struct rp_error err;
};

void rp_error(struct rp_param *param, int line, int col, char const *msg);

struct r_expression* rp_new_expr_def(struct r_definition *def);
struct r_expression* rp_new_expr_stmt(struct r_statment *stmt);

struct r_definition* rp_new_def(const char *name);
struct r_definition* rp_new_def2(const char *name, const char *type);
struct r_definition* rp_new_def3(const char *name, const char *from, const char *to);

struct r_statment* rp_new_stmt(const char *name, const char *name2);

#endif /* PARSER_INTERNAL_H_ */
