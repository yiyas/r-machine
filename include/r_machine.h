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

struct r_boolean_expression {
    enum {
        RB_AND, RB_OR, RB_NOT, RB_VALUE, RB_VARIABLE
    } type;

    union {
        struct r_boolean_expression *two[2];
        struct r_boolean_expression *one;
        R_BOOLEAN value;
        const char *name;
    } data;
};

#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
