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

typedef enum {
    RB_VALUE, RB_VARIABLE, RB_NOT, RB_AND, RB_OR
} R_SENTENCE_TYPE;

struct r_logic_sentence;

struct r_logic_sentences {
    struct r_logic_sentence **array;
    uint32_t count;
    uint32_t size;
};

struct r_proposition {
    struct r_logic_sentences premise;
    struct r_logic_sentences conclusion;
};

struct r_error;

int r_init();
void r_destroy();

void r_error_destroy(struct r_error *err);

struct r_logic_sentence* r_sentence_parse(const char *expr, struct r_error **err);
int r_sentence_print(char **str, const struct r_logic_sentence *sentence);
int r_sentence_cmp(const struct r_logic_sentence *st1, const struct r_logic_sentence *st2);
void r_sentence_destroy(struct r_logic_sentence *sentence);

const struct r_logic_sentence* r_dfs_next(const struct r_logic_sentence *root, const struct r_logic_sentence *st);

int r_sentences_add(struct r_logic_sentences *sts, struct r_logic_sentence *st);
int r_sentences_add_exp(struct r_logic_sentences *sts, const char *exp, struct r_error **err);
void r_sentences_clean(struct r_logic_sentences *sts);
void r_proposition_clean(struct r_proposition *p);

int r_convert(struct r_logic_sentence *sentence, const struct r_logic_sentence *from, const struct r_logic_sentence *to,
        struct r_error **err);

int r_calculate(struct r_logic_sentences *premise, const struct r_proposition *rule, struct r_error **err);

#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
