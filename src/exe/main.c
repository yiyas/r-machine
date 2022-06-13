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
#include <stdlib.h>

#include "../r_internal.h"

int main(int argc, char *argv[])
{
    struct r_expression *exp = NULL;

    if(argc != 2) {
        LOG_ERR("Usage: %s <expression>", argv[0]);
        return -1;
    }

    LOG_INF("Start");

    exp = r_parse(argv[1]);
    CHECK_UNLIKELY_RT(!exp, -1);

    if(exp->type == R_ERR) {
        LOG_INF("Failed to parse: %s, error: %s", argv[1], exp->data.err->msg);
    } else {
        LOG_INF("Succeeded to parse: %s", argv[1]);
    }

    r_destroy(exp);

    return 0;
}
