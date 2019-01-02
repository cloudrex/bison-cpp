#include <stdio.h>
#include <stdlib.h>
#include <iostream> // For 'cout'
#include "llvm/IR/Module.h"

int yyparse (void);

using namespace std;
using namespace llvm;

int main() {
    printf("--- Program start ---\n");
    yyparse();
    printf("--- Program end ---\n");

    return 0;
}