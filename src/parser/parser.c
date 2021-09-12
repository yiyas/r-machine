/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include <stdio.h>

#include "parser.h"

void yyerror(YYLTYPE *yyl, void *scanner, char const *msg) {
    (void) scanner;

    printf("Error occurred at %d-%d: %s\n", yyl->first_line, yyl->first_column, msg);
}

