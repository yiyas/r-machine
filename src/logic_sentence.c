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
#include <libal/pub.h>
#include <libal/dict.h>
#include "r_internal.h"
#include "parser/parser_bison.h"
#include "parser/parser_flex.h"

int r_sentence_print(char **str, const struct r_logic_sentence *sentence) {
    CHECK_NULLARG2_RT(str, sentence, -1);

    switch (sentence->type) {
    case RB_VALUE:
        *str = strdup(sentence->data.value ? "true" : "false");
        break;
    default:
        LOG_ERR("unlikely branch!");
    }

    return *str == NULL ? 1 : 0;
}

void r_sentence_destroy(struct r_logic_sentence *sentence) {
    if (!sentence) {
        return;
    }

    switch (sentence->type) {
    case RB_VARIABLE:
        al_dict_remove(sentence->data.name);
        break;
    case RB_AND:
    case RB_OR:
        r_sentence_destroy(sentence->data.two[0]);
        r_sentence_destroy(sentence->data.two[1]);
        break;
    case RB_NOT:
        r_sentence_destroy(sentence->data.one);
        break;
    default:
        break;
    }

    free(sentence);
}

