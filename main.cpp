#include <stdio.h>
#include <stdlib.h>
#include <iostream> // For 'cout'

int yyparse (void);

using namespace std;

int main() {
    printf("--- Program start ---\n");
    yyparse();
    printf("--- Program end ---\n");

    return 0;
}