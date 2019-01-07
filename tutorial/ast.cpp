#include "ast.h"
#include <string>
#include <memory>
#include <vector>

// Base class for all expression nodes
class ExprAST
{
  public:
    virtual ~ExprAST() {}
};

// Expression class for numeric literals (ex. '1.0')
class NumberExprAST : public ExprAST
{
    double Val;

  public:
    NumberExprAST(double Val) : Val(Val) {}
};

// Expression class for referencing a variable (ex. 'a')
class VariableExprAST : public ExprAST
{
    std::string Name;

  public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

// Expression class for a binary operator
class BinaryExprAST : public ExprAST
{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

// Expression class for function calls
class CallExprAST : public ExprAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

  public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}
};

// Representation of a function's prototype
class PrototypeAST
{
    std::string Name;
    std::vector<std::string> Args;

  public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args) : Name(name), Args(std::move(Args)) {}

    const std::string &getName() const
    {
        return Name;
    }
};

// Representation of a function definition
class FunctionAST
{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

  public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)) {}
};