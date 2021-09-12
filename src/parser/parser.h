/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "../../bison-flex/parser_bison.h"

void yyerror (YYLTYPE *yyl, void *scanner, char const *msg);


#endif /* PARSER_H_ */
