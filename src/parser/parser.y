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

static void yyerror(YYLTYPE *yyl, void *scanner, char const *msg);
%}

%define api.pure full
%locations

%lex-param {void * scanner}
%parse-param {void * scanner}

%union{
    R_BOOLEAN bool;
    const char *string;
    struct r_boolean_expression expr;
}

%token <bool> BOOLEAN
%token <string> NAME
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

logic_expression: BOOLEAN { parser_init_bool(&($$), $1); }
  |NAME { if(parser_init_variable(&($$), $1)) { YYABORT; } }
  |'(' logic_expression ')' { $$ = $2; }
  |logic_expression AND logic_expression { if(parser_init_and(&($$), &($1), &($3))) { YYABORT; } }
  |logic_expression OR logic_expression { if(parser_init_or(&($$), &($1), &($3))) { YYABORT; } }
  |NOT logic_expression { if(parser_init_not(&($$), &($2))) { YYABORT; } }
  
  
  
%%

static void yyerror(YYLTYPE *yyl, void *scanner, char const *msg) {
    (void) scanner;
    parser_error(yyl->first_line, yyl->first_column, msg);
}
