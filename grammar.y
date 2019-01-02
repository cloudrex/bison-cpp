%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "grammar.hh"
    #include "lex.h"

    // Required
    void yyerror(const char *s);
%}

%require "3.2"

%start global

%token hello

%%
global: hello { printf("GOODBYE!\n"); }
      ;
%%

// YY methods
int yywrap()
{
    return 1;
}

void yyerror(char const *x) {
    printf("--- Syntax error: %s ---\n", x);
    exit(1);
}