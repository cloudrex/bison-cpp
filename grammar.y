%{
    int yylex(void);
    void yyerror(char const *);
%}

%require "3.2"
%language "c++"

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