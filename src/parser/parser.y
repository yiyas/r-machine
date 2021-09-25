%{
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

static void yyerror(YYLTYPE *yyl, void *scanner, struct r_logic_sentence **stc, char const *msg);
%}

%define api.pure full
%locations

%lex-param {void * scanner}
%parse-param {void * scanner} {struct r_logic_sentence **stc}

%union{
    R_BOOLEAN bool;
    const char *string;
    struct r_logic_sentence *expr;
}

%token <bool> BOOLEAN
%token <string> NAME
%token NOT
%token AND
%token OR

%left OR
%left AND
%right NOT

%type <expr> logic_expression

%start r_expression

%%
r_expression: 
  logic_expression { *stc = $1; }

logic_expression: BOOLEAN { $$ = parser_init_bool($1); }
  |NAME { $$ = parser_init_variable($1); }
  |'(' logic_expression ')' { $$ = $2; }
  |logic_expression AND logic_expression { $$ = parser_init_and($1, $3); }
  |logic_expression OR logic_expression { $$ = parser_init_or($1, $3); }
  |NOT logic_expression { $$ = parser_init_not($2); }
  
  
  
%%

static void yyerror(YYLTYPE *yyl, void *scanner, struct r_logic_sentence **stc, char const *msg) {
    (void) scanner;
    (void) stc;
    parser_error(yyl->first_line, yyl->first_column, msg);
}
