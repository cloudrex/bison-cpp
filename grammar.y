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

%token T_TRUE
%token T_FALSE
%token T_END

%%
statement: T_TRUE T_END { ; }
         | T_FALSE T_END { ; }
         ;

global: statement { ; }
      | global statement { ; }
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