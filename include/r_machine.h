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

struct r_definition {
    const char *name;
};

struct r_statment {
    const char *name;
};

struct r_expression {
    enum {
        R_DEF, R_STMT
    } type;

    union {
        struct r_definition *def;
        struct r_statment *stmt;
    } data;
};

#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
