/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include "parser_internal.h"
#include "parser_bison.h"
#include "parser_flex.h"
#include "../r_internal.h"

struct r_expression* r_parse(const char *expr) {
    int rc;
    yyscan_t scanner = NULL;
    YY_BUFFER_STATE buffer = NULL;
    struct r_expression *exp = NULL;

    rc = yylex_init(&scanner);
    CHECK_UNLIKELY_GOTO(rc, finish);

    buffer = yy_scan_string(expr, scanner);
    CHECK_UNLIKELY_GOTO(!buffer, finish);

    rc = yyparse(scanner, &exp);
    if (rc) {
        LOG_INF("Failed to parse expression: %s", expr);
        goto finish;
    }

    finish:
    yy_delete_buffer(buffer, scanner);
    yylex_destroy(scanner);
    if(rc) {
        r_destroy(exp);
        exp = NULL;
    }
    return exp;
}

void r_destroy(struct r_expression *exp)
{
    free(exp);
}
