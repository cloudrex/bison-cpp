%{
    #include "grammar.hh"

    // Required
    void yyerror(const char *s);
%}

%%
"hello" { return hello; }
. { yyerror("Unexpected character"); }
%%