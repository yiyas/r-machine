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
#include <stdarg.h>

#include "r_internal.h"

void r_log(const char *level, const char *file, int line, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);

    printf("[%s] %s:%d ", level, file, line);
    vprintf(fmt, ap);
    printf("\n");

    va_end(ap);
}
