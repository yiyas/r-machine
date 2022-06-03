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

static void yyerror(YYLTYPE *yyl, void *scanner, struct rp_param *param, char const *msg);
%}

%define api.pure full
%locations

%lex-param {void * scanner}
%parse-param {void * scanner} {struct rp_param *param}

%union{
    const char *string;
    struct r_definition *def;
    struct r_statment *stmt;
}

%token <string> NAME
%token DOUBLE_COLON
%token COLON_EQUAL
%token ARROW
%token SPACE
%token UNEXPECTED_CHAR

%type <def> definition
%type <stmt> statment
%type <def> simple_definition
%type <def> two_colon_definition
%type <def> arrow_definition

%start r_expression

%%
r_expression: definition { param->expr = rp_new_expr_def($1); }
  |statment { param->expr = rp_new_expr_stmt($1); }
;

definition: simple_definition { $$ = $1; }
  |two_colon_definition { $$ = $1; }
  |arrow_definition { $$ = $1; }
;

simple_definition: NAME {
    $$ = rp_new_def($1);
}
;

two_colon_definition: NAME DOUBLE_COLON NAME {
    $$ = rp_new_def2($1, $3);
}
;

arrow_definition: NAME COLON_EQUAL NAME ARROW NAME {
    $$ = rp_new_def3($1, $3, $5);
}
;

statment: NAME SPACE NAME {
    $$ = rp_new_stmt($1, $3);
}
;

%%

static void yyerror(YYLTYPE *yyl, void *scanner, struct rp_param *param, char const *msg) {
    (void) scanner;
    rp_error(param, yyl->first_line, yyl->first_column, msg);
}
