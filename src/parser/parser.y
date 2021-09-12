%{
/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
 
#include "../src/parser/parser.h"
#include "parser_bison.h"
#include "parser_flex.h"
%}

%define api.pure full
%locations

%lex-param {void * scanner}
%parse-param {void * scanner}

%union{
    int bool;
}

%token BOOLEAN
%token NOT
%token AND
%token OR

%start r_expression

%%
r_expression: /* nothing */
  |r_expression logic_expression {  }

logic_expression: BOOLEAN
  |logic_expression AND logic_expression {}
  |logic_expression OR logic_expression {}
  |NOT logic_expression {}
  
  
  
%%

