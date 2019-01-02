#include <stdio.h>
#include <stdlib.h>

int yyparse (void);

int main() {
    printf("Hello from Main function!");
    yyparse();

    return 0;
}