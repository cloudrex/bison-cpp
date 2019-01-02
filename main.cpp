#include <stdio.h>
#include <stdlib.h>

int yyparse (void);

int main() {
    printf("--- Program start ---\n");
    yyparse();
    printf("--- Program end ---\n");

    return 0;
}