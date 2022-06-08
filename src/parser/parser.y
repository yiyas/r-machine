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

%type <string> list_exp
%type <string> finite_set_exp
%type <string> set_exp

%type <def> definition
%type <stmt> statment
%type <def> simple_definition
%type <def> set_definition
%type <def> func_definition

%start r_expression

%%
list_exp: NAME { }
	| list_exp ',' NAME { }

finite_set_exp: '{' list_exp '}' {  }

set_exp: NAME
	|'{' list_exp '}' {  }

r_expression: definition { param->expr = rp_new_expr_def($1); }
  |statment { param->expr = rp_new_expr_stmt($1); }
;

definition: simple_definition { $$ = $1; }
  |set_definition { $$ = $1; }
  |func_definition { $$ = $1; }
;

simple_definition: NAME { $$ = rp_new_def($1); }
;

set_definition: NAME COLON_EQUAL finite_set_exp { $$ = rp_new_def2($1, $3); }
;

func_definition: NAME DOUBLE_COLON set_exp ARROW set_exp { $$ = rp_new_def3($1, $3, $5); }
;

statment: NAME SPACE NAME { $$ = rp_new_stmt($1, $3); }
;

%%

static void yyerror(YYLTYPE *yyl, void *scanner, struct rp_param *param, char const *msg) {
    (void) scanner;
    rp_error(param, yyl->first_line, yyl->first_column, msg);
}
