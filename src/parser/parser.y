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
    const char *string;
    struct r_boolean_expression expr;
}

%token BOOLEAN
%token NAME
%token NOT
%token AND
%token OR

%left OR
%left AND
%right NOT

%type <expr> r_expression
%type <expr> logic_expression

%start r_expression

%%
r_expression: 
  logic_expression { $$ = $1; }

logic_expression: BOOLEAN {  }
  |NAME {  }
  |'(' logic_expression ')' { $$ = $2; }
  |logic_expression AND logic_expression { $$ = $1; }
  |logic_expression OR logic_expression { $$ = $1; }
  |NOT logic_expression { $$ = $2; }
  
  
  
%%

