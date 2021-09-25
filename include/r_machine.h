/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef R_MACHINE_H_
#define R_MACHINE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RB_FALSE = 0, RB_TRUE = 1
} R_BOOLEAN;

struct r_logic_sentence {
    enum {
        RB_AND, RB_OR, RB_NOT, RB_VALUE, RB_VARIABLE
    } type;

    union {
        struct r_logic_sentence *two[2];
        struct r_logic_sentence *one;
        R_BOOLEAN value;
        const char *name;
    } data;
};

struct r_proposition {
    struct r_logic_sentence **premises;
    uint32_t premise_size;

    struct r_logic_sentence **conclusions;
    uint32_t conclusion_size;
};

struct r_error;
struct r_rule;

struct r_logic_sentence* r_tree_parse(const char **expr, struct r_error *err);
char* r_tree_print(const struct r_logic_sentence *tree);
void r_tree_destroy(struct r_logic_sentence *tree);

int r_convert(struct r_logic_sentence *tree, const struct r_rule *rule, struct r_error *err);


#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
