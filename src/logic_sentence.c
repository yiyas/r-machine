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

#define is_left_operand_need_backets(t, st) (st->type <= t ? 0 : 1)
#define is_right_operand_need_backets(t, st) (st->type < t ? 0 : 1)

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

    if (is_left_operand_need_backets(type, l) && is_right_operand_need_backets(type, r)) {
        rc = asprintf(str, "(%s) %s (%s)", lstr, (type == RB_AND ? "and" : "or"), rstr);
    } else if (is_left_operand_need_backets(type, l)) {
        rc = asprintf(str, "(%s) %s %s", lstr, (type == RB_AND ? "and" : "or"), rstr);
    } else if (is_right_operand_need_backets(type, r)) {
        rc = asprintf(str, "%s %s (%s)", lstr, (type == RB_AND ? "and" : "or"), rstr);
    } else {
        rc = asprintf(str, "%s %s %s", lstr, (type == RB_AND ? "and" : "or"), rstr);
    }
    free(lstr);
    free(rstr);
    CHECK_INTERR_RT(rc < 0, -1);

    return 0;
}

static int print_one_operand(char **str, R_SENTENCE_TYPE type, const struct r_logic_sentence *r) {
    int rrc, rc;
    char *rstr = NULL;

    (void) type;

    rrc = r_sentence_print(&rstr, r);
    if (rrc) {
        return 1;
    }

    if (!is_left_operand_need_backets(type, r)) {
        rc = asprintf(str, "!%s", rstr);
    } else {
        rc = asprintf(str, "!(%s)", rstr);
    }
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
    case RB_NOT:
        rc = print_one_operand(str, sentence->type, sentence->data.one);
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

const struct r_logic_sentence* r_dfs_next(const struct r_logic_sentence *root, const struct r_logic_sentence *st) {
    const struct r_logic_sentence *parent;

    if (st->type == RB_AND || st->type == RB_OR) {
        return st->data.two[0];
    }
    if (st->type == RB_NOT) {
        return st->data.one;
    }

    for (parent = st->parent; parent; st = parent, parent = parent->parent) {
        if (st == root) {
            return NULL;
        }
        if (parent->type == RB_AND || parent->type == RB_OR) {
            if (st == parent->data.two[0]) {
                return parent->data.two[1];
            }
        }
    }

    return NULL;
}

int r_sentence_cmp(const struct r_logic_sentence *st1, const struct r_logic_sentence *st2) {
    if (st1 == st2) {
        return 0;
    }

    if (!st1 || !st2) {
        return 1;
    }

    if (st1->type != st2->type) {
        return 1;
    }

    switch(st1->type) {
    case RB_NOT:
        return r_sentence_cmp(st1->data.one, st2->data.one);
    case RB_AND:
    case RB_OR:
        return r_sentence_cmp(st1->data.two[0], st2->data.two[0]) || r_sentence_cmp(st1->data.two[1], st2->data.two[1]);
    case RB_VALUE:
        return st1->data.value == st2->data.value;
    case RB_VARIABLE:
        return strcmp(st1->data.name, st2->data.name);
    default:
        LOG_ERR("unlikely!");
        return  -1;
    }
}

