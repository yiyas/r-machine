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
#include <r_machine.h>
#include <libal/pub.h>
#include <libal/dict.h>
#include "r_internal.h"
#include "parser/parser_bison.h"
#include "parser/parser_flex.h"

static int print_two_operands(char **str, R_SENTENCE_TYPE type, const struct r_logic_sentence *l,
        const struct r_logic_sentence *r) {
    int lrc, rrc, rc;
    char *lstr = NULL, *rstr = NULL;

    lrc = r_sentence_print(&lstr, l);
    rrc = r_sentence_print(&rstr, r);

    if (lrc || rrc) {
        free(lstr);
        free(rstr);
        return 1;
    }

    rc = asprintf(str, "%s %s %s", lstr, (type == RB_AND ? "and" : "or"), rstr);
    free(lstr);
    free(rstr);
    CHECK_INTERR_RT(rc < 0, -1);

    return 0;
}

int r_sentence_print(char **str, const struct r_logic_sentence *sentence) {
    int rc = 0;

    CHECK_NULLARG2_RT(str, sentence, -1);

    switch (sentence->type) {
    case RB_VALUE:
        *str = strdup(sentence->data.value ? "true" : "false");
        rc = (*str == NULL ? 1 : 0);
        break;
    case RB_VARIABLE:
        *str = strdup(sentence->data.name);
        rc = (*str == NULL ? 1 : 0);
        break;
    case RB_AND:
    case RB_OR:
        rc = print_two_operands(str, sentence->type, sentence->data.two[0], sentence->data.two[1]);
        break;
    default:
        LOG_ERR("unlikely branch!");
        rc = -1;
        break;
    }

    return rc;
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

