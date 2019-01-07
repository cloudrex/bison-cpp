#include "llvm/ADT/STLExtras.h"

#include <string>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <map>
#include <memory>
#include <vector>

enum Token {
    tok_eof = -1,

    // Commands
    tok_def = -2,
    tok_extern = -3,

    // Primary
    tok_identifier = -4,
    tok_number = -5
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal; // Filled in if tok_number

static int getToken() {
    static int LastChar = ' ';

    // Skip whitespace
    while (isspace(LastChar)) {
        LastChar = getchar();
    }

    // Recognize identifiers and keywords (ex. 'def')
    if (isalpha(LastChar)) { // Identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;

        while (isalnum((LastChar = getchar()))) {
            IdentifierStr += LastChar;
        }

        if (IdentifierStr == "def") {
            return tok_def;
        }
        else if (IdentifierStr == "extern") {
            return tok_extern;
        }

        return tok_identifier;
    }
    // Recognize numeric values (numbers) | TODO: Needs error validation, ex. 1.23.45.67 -> 1.23
    else if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
        std::string NumStr;

        do {
            NumStr += LastChar;
            LastChar = getchar();
        }
        while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);

        return tok_number;
    }
    // Recognize single-line comments (#)
    else if (LastChar == '#') {
        do {
            LastChar = getchar();
        }
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            return getToken();
        }
    }

    // Recognize end-of-file
    if (LastChar == EOF) {
        return tok_eof;
    }

    // Otherwise, return ASCII value
    int ThisChar = LastChar;

    LastChar = getchar();

    return ThisChar;
}