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

void parser_error(int line, int col, char const *msg);

struct r_logic_sentence* parser_init_bool(R_BOOLEAN v);

struct r_logic_sentence* parser_init_variable(const char *name);

struct r_logic_sentence* parser_init_and(struct r_logic_sentence *l, struct r_logic_sentence *r);
struct r_logic_sentence* parser_init_or(struct r_logic_sentence *l, struct r_logic_sentence *r);
struct r_logic_sentence* parser_init_xor(struct r_logic_sentence *l, struct r_logic_sentence *r);
struct r_logic_sentence* parser_init_if(struct r_logic_sentence *l, struct r_logic_sentence *r);
struct r_logic_sentence* parser_init_iff(struct r_logic_sentence *l, struct r_logic_sentence *r);

struct r_logic_sentence* parser_init_not(struct r_logic_sentence *tgt);

#endif /* PARSER_INTERNAL_H_ */
