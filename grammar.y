%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "grammar.hh"
    #include "lex.h"

    //extern "C" int yylex(void);

    
    void yyerror(const char *s);
%}

%require "3.2"

%start global

%%
global: 
      ;
%%

// YY methods
int yywrap()
{
    return 1;
}

void yyerror(char const *x) {
    printf("Error %s\n", x);
    exit(1);
}