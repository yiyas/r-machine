/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

%{
#include "../src/parser/parser.h"
#include "parser_bison.h"
%}

%option noyywrap nounput noinput reentrant bison-bridge bison-locations

%%
"(" |
")" |
"[" |
"]" |
"{" |
"}"     { return yytext[0]; }

"true"  { yylval->bool = 1; return BOOLEAN; }
"false" { yylval->bool = 0; return BOOLEAN; }

"and" { return AND; }
"or" { return OR; }
"!" { return NOT; }

[A-Za-z][A-Za-z0-9]*  { yylval->string = yytext; return NAME; }

%%

