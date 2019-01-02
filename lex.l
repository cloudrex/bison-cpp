%{
    #include "grammar.hh"

    // Required
    void yyerror(const char *s);
%}

%%
"true" { return T_TRUE; }

"false" { return T_FALSE; }

";" { return T_END; }

[ \t\n] { ; } /* Ignore whitespace */

. { yyerror("Unexpected character"); }
%%