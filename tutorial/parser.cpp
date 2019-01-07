#include "llvm/ADT/STLExtras.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include <vector>

// Table of precendence for each binary operator
std::map<char, int> BinopPrecedence;

auto LHS = llvm::make_unique<VariableExprAST>("x");
auto RHS = llvm::make_unique<BinaryExprAST>('+', std::move(LHS), std::move(RHS));

static int CurTok;

int getNextToken()
{
    return CurTok = getToken();
}

static int GetTokPrecedence()
{
    if (isascii(CurTok))
    {
        return -1;
    }

    // Ensure it's a declared binary operator
    int TokPrecedence = BinopPrecedence[CurTok];

    if (TokPrecedence <= 0)
    {
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
            if (auto Arg = ParseExpression())
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

// expression
//  ::= primary binoprhs
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto LHS = ParsePrimary();

    if (!LHS)
    {
        return nullptr;
    }

    return ParseBinOpRHS(0, std::move(LHS));
}

// binoprhs
//  ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrecedence, std::unique_ptr<ExprAST> LHS)
{
    // If this is a binop, find it's precedence
    while (1)
    {
        int TokPrecedence = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done
        if (TokPrecedence < ExprPrecedence)
        {
            return LHS;
        }

        // We know this is a binop
        int BinOp = CurTok;

        // Eat binop
        getNextToken();

        // Parse the primary expression after the binary operator
        auto RHS = ParsePrimary();

        if (!RHS)
        {
            return nullptr;
        }

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS
        int NextPrecedence = GetTokPrecedence();

        if (TokPrecedence < NextPrecedence)
        {
            RHS = ParseBinOpRHS(TokPrecedence + 1, std::move(RHS));

            if (!RHS)
            {
                return nullptr;
            }
        }

        // Merge LHS/RHS
        LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// prototype
//  ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if (CurTok != tok_identifier)
    {
        return LogErrorP("Expecting function name in prototype");
    }

    std::string FunctionName = IdentifierStr;

    getNextToken();

    if (CurTok != '(')
    {
        return LogErrorP("Expecting '(' in prototype");
    }

    // Read the list of argument names
    std::vector<std::string> ArgNames;

    while (getNextToken() == tok_identifier)
    {
        ArgNames.push_back(IdentifierStr);
    }

    if (CurTok != ')')
    {
        return LogErrorP("Expected ')' in prototype");
    }

    // Eat ')'
    getNextToken();

    return llvm::make_unique<PrototypeAST>(FunctionName, std::move(ArgNames));
}

// definition
//  ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
    // Eat def.
    getNextToken();

    auto Proto = ParsePrototype();

    if (!Proto)
    {
        return nullptr;
    }
    else if (auto Expr = ParseExpression())
    {
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(Expr));
    }

    return nullptr;
}

// external
//  ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern()
{
    // Eat 'extern'
    getNextToken();

    return ParsePrototype();
}

// toplevelexpr
//  ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if (auto Expr = ParseExpression())
    {
        // Make anonymous prototype
        auto Proto = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());

        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(Expr));
    }

    return nullptr;
}

static void HandleDefinition()
{
    if (ParseDefinition())
    {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
    {
        // Skip token for error recovery
        getNextToken();
    }
}

static void HandleExtern()
{
    if (ParseExtern())
    {
        fprintf(stderr, "Parsed an extern\n");
    }
    else
    {
        // Skip token for error recovery
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function
    if (ParseTopLevelExpr())
    {
        fprintf(stderr, "Parsed a top-level expression\n");
    }
    else
    {
        // Skip token for error recovery
        getNextToken();
    }
}

// top
//  ::= definition | external | expression | ';'
void MainLoop()
{
    while (1)
    {
        fprintf(stderr, "ready> ");

        switch (CurTok)
        {
        case tok_eof:
            return;
        // Ignore top-level semicolons
        case ';':
            getNextToken();

            break;
        case tok_def:
            HandleDefinition();

            break;
        case tok_extern:
            HandleExtern();

            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}