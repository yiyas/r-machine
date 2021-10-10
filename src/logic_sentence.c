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

#define is_left_operand_need_backets(t, st) (st_type_get_priority(st->type) <= st_type_get_priority(t) ? 0 : 1)
#define is_right_operand_need_backets(t, st) (st_type_get_priority(st->type) < st_type_get_priority(t) ? 0 : 1)

inline int st_type_get_operand_count(R_SENTENCE_TYPE type) {
    if (type == RB_NOT) {
        return 1;
    } else if (type > RB_NOT) {
        return 2;
    } else {
        return 0;
    }
}

inline static int st_type_get_priority(R_SENTENCE_TYPE type) {
    if (type == RB_XOR) {
        return RB_OR;
    }

    return type;
}

inline static const char* st_type_get_name(R_SENTENCE_TYPE type) {
    switch(type) {
    case RB_NOT:
        return "!";
    case RB_AND:
        return "and";
    case RB_OR:
        return "or";
    case RB_XOR:
        return "xor";
    case RB_IF:
        return "=>";
    case RB_IFF:
        return "<=>";
    default:
        return "";
    }
}

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
        rc = asprintf(str, "(%s) %s (%s)", lstr, st_type_get_name(type), rstr);
    } else if (is_left_operand_need_backets(type, l)) {
        rc = asprintf(str, "(%s) %s %s", lstr, st_type_get_name(type), rstr);
    } else if (is_right_operand_need_backets(type, r)) {
        rc = asprintf(str, "%s %s (%s)", lstr, st_type_get_name(type), rstr);
    } else {
        rc = asprintf(str, "%s %s %s", lstr, st_type_get_name(type), rstr);
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
    default:
        if(st_type_get_operand_count(sentence->type) == 2) {
            rc = print_two_operands(str, sentence->type, sentence->data.two[0], sentence->data.two[1]);
        } else if (st_type_get_operand_count(sentence->type) == 1) {
            rc = print_one_operand(str, sentence->type, sentence->data.one);
        } else {
            LOG_UNLIKELY();
            rc = -1;
        }
        break;
    }

    return rc;
}

void r_sentence_destroy(struct r_logic_sentence *sentence) {
    if (!sentence) {
        return;
    }

    if (st_type_get_operand_count(sentence->type) == 2) {
        r_sentence_destroy(sentence->data.two[0]);
        r_sentence_destroy(sentence->data.two[1]);
    } else if (st_type_get_operand_count(sentence->type) == 1) {
        r_sentence_destroy(sentence->data.one);
    } else if (sentence->type == RB_VARIABLE) {
        al_dict_remove(sentence->data.name);
    }

    free(sentence);
}

const struct r_logic_sentence* r_dfs_next(const struct r_logic_sentence *root, const struct r_logic_sentence *st) {
    const struct r_logic_sentence *parent;

    if (st_type_get_operand_count(st->type) == 2) {
        return st->data.two[0];
    }
    if (st_type_get_operand_count(st->type) == 1) {
        return st->data.one;
    }

    for (parent = st->parent; parent; st = parent, parent = parent->parent) {
        if (st == root) {
            return NULL;
        }
        if (st_type_get_operand_count(parent->type) == 2) {
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

    switch (st1->type) {
    case RB_VALUE:
        return st1->data.value == st2->data.value;
    case RB_VARIABLE:
        return strcmp(st1->data.name, st2->data.name);
    default:
        if (st_type_get_operand_count(st1->type) == 2) {
            return r_sentence_cmp(st1->data.two[0], st2->data.two[0])
                    || r_sentence_cmp(st1->data.two[1], st2->data.two[1]);
        } else if (st_type_get_operand_count(st1->type) == 1) {
            return r_sentence_cmp(st1->data.one, st2->data.one);
        } else {
            LOG_UNLIKELY();
            return -1;
        }
    }
}

struct r_logic_sentence* r_sentence_dup(const struct r_logic_sentence *st) {
    struct r_logic_sentence *dup;

    if (!st) {
        return NULL;
    }

    dup = calloc(1, sizeof(*dup));
    CHECK_NOMEM_RT(dup, NULL);

    (void) memcpy(dup, st, sizeof(*dup));

    dup->parent = NULL;

    if (st_type_get_operand_count(dup->type) == 2) {
        dup->data.two[0] = r_sentence_dup(st->data.two[0]);
        if (!dup->data.two[0]) {
            goto error;
        }
        dup->data.two[0]->parent = dup;
        dup->data.two[1] = r_sentence_dup(st->data.two[1]);
        if (!dup->data.two[1]) {
            r_sentence_destroy(dup->data.two[0]);
            goto error;
        }
        dup->data.two[1]->parent = dup;
    } else if (st_type_get_operand_count(dup->type) == 1) {
        dup->data.one = r_sentence_dup(st->data.one);
        if (!dup->data.one) {
            goto error;
        }
        dup->data.one->parent = dup;
    } else if (dup->type == RB_VARIABLE){
        dup->data.name = al_dict_add(st->data.name);
        if (!dup->data.name) {
            goto error;
        }
    }

    return dup;

    error:
    free(dup);
    return NULL;
}

