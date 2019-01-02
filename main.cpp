#include <stdio.h>
#include <stdlib.h>

extern "C" int yyparse (void);

int main() {
    yyparse();

    return 0;
}