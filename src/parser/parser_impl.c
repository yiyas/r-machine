/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include <r_machine.h>
#include <libal/pub.h>

#include "parser_bison.h"
#include "parser_flex.h"

struct r_logic_sentence* r_sentence_parse(const char *expr, struct r_error **err) {
    int rc = 0;
    yyscan_t scanner = NULL;
    YY_BUFFER_STATE buffer = NULL;
    struct r_logic_sentence *stc = NULL;

    (void) err;

    rc = yylex_init(&scanner);
    CHECK_INTERR_RT(rc, NULL);

    buffer = yy_scan_string(expr, scanner);
    CHECK_INTERR_GOTO(!buffer, finish);

    rc = yyparse(scanner, &stc);
    if (rc) {
        goto finish;
    }

    finish: yy_delete_buffer(buffer, scanner);
    yylex_destroy(scanner);
    return stc;
}
