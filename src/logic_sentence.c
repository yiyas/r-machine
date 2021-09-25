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
#include "parser/parser_bison.h"
#include "parser/parser_flex.h"

struct r_logic_sentence* r_sentence_parse(const char *expr, struct r_error **err) {
    return NULL;
}

int r_sentence_print(char **str, const struct r_logic_sentence *sentence) {
    return 0;
}

void r_sentence_destroy(struct r_logic_sentence *sentence) {
}

