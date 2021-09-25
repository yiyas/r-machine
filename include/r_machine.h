/**
 * Copyright (c) 2015 - 2021 YIYAS
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

struct r_logic_sentences {
    struct r_logic_sentence **array;
    uint32_t size;
};

struct r_proposition {
    struct r_logic_sentence premise;
    struct r_logic_sentence conclusion;
};

struct r_error;

void r_error_destroy(struct r_error *err);

struct r_logic_sentence* r_sentence_parse(const char *expr, struct r_error **err);
int r_sentence_print(char **str, const struct r_logic_sentence *sentence);
void r_sentence_destroy(struct r_logic_sentence *sentence);

int r_calculate(const struct r_logic_sentences *premise, const struct r_proposition *rule,
        struct r_logic_sentences *conclusion, struct r_error **err);

#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
