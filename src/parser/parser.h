/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <r_machine.h>

void parser_error(int line, int col, char const *msg);

void parser_init_bool(struct r_boolean_expression *expr, R_BOOLEAN v);

int parser_init_variable(struct r_boolean_expression *expr, const char *name);

int parser_init_and(struct r_boolean_expression *expr, const struct r_boolean_expression *l,
        struct r_boolean_expression *r);

int parser_init_or(struct r_boolean_expression *expr, const struct r_boolean_expression *l,
        struct r_boolean_expression *r);

int parser_init_not(struct r_boolean_expression *expr, const struct r_boolean_expression *tgt);

#endif /* PARSER_H_ */
