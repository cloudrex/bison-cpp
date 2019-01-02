%{
    #include "grammar.hh"
%}

%option c++

%%
"hello" { printf("GOODBYE\n"); }
%%