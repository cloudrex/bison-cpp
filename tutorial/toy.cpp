#include <iostream>
#include "parser.h"

int main()
{
    // Install standard binary operator precedence ('1' being lowest)
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;

    // Prime the first token
    fprintf(stderr, "ready> ");

    getNextToken();

    // Run interpreter loop
    //MainLoop();

    std::cout << "Program finished" << std::endl;

    return 0;
}