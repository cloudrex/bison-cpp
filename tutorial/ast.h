#pragma once

class ExprAST
{
};

class NumberExprAST
{
public:
  NumberExprAST(double Val);
};

class VariableExprAST
{
public:
  VariableExprAST(const std::string &Name);
};

class BinaryExprAST
{
public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS);
};

class CallExprAST
{
public:
  CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args);
};

class PrototypeAST
{
public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args) : Name(name), Args(std::move(Args)) {}

  std::string &getName();
};

class FunctionAST
{
public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body);
};