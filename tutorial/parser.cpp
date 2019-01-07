#include <llvm>
#include "ast.h"

// Table of precendence for each binary operator
static std::map<char, int> BinopPrecedence;

auto LHS = llvm : make_unique<VariableExprAST>("x");
auto RHS = llvm : make_unique<BinaryExprAST>('+', std::move(LHS), std::move(RHS));

static int CurTok;

static int getNextToken()
{
    return CurTok = getToken();
}

static int GetTokPrecedence()
{
    if (isascii(CurTok)) {
        return -1;
    }

    // Ensure it's a declared binary operator
    int TokPrecedence = BinopPrecedence[CurTok];

    if (TokPrecedence <= 0) {
        return -1;
    }

    return TokPrecedence;
}

// Error handling helper
std::unique_ptr<ExprAST> LogError(const char *str)
{
    fprintf(stderr, "LogError: %s\n", str);

    return nullptr;
}

// Error handling helper
std::unique_ptr<PrototypeAST> LogErrorP(const char *str)
{
    LogError(str);

    return nullptr;
}

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);

    // Consume the number
    getNextToken();

    return std::move(Result);
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr()
{
    // Eat '('
    getNextToken();

    auto V = ParseExpression();

    if (!V)
    {
        return nullptr;
    }

    if (CurTok != ')')
    {
        return LogError("Expecting ')'");
    }

    // Eat ')'
    getNextToken();

    return V;
}

// identifierexpr
//  ::= identifier
//  ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
    std::string IdName = IdentifierStr;

    // Eat identifier
    getNextToken();

    // Simple variable ref.
    if (CurTok != '(')
    {
        return llvm::make_unique<VariableExprAST>(IdName);
    }

    // Call -> Eat '('
    getNextToken();

    std::vector<std::unique_ptr<ExprAST>> Args;

    if (CurTok != ')')
    {
        while (1)
        {
            if (autoArg = ParseExpression())
            {
                Args.push_back(std::move(Arg));
            }
            else
            {
                return nullptr;
            }

            if (CurTok == ')')
            {
                break;
            }
            else if (CurTok != ',')
            {
                return LogError("Expecting ')' or ',' in argument list");
            }

            getNextToken();
        }
    }

    // Call -> Eat ')'
    getNextToken();

    return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
}

// primary
//  ::= identifierexpr
//  ::= numberexpr
//  ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary()
{
    switch (CurTok)
    {
    default:
        return LogError("Unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}