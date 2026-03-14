#pragma once
#include <vector>
#include <memory>
#include "statement/stmt.hpp"

class Callable;
class Interpreter;

class Function : public Callable
{
private:
    FunctionDeclStmt *declaration;
    std::shared_ptr<Environment> closure;

public:
    int arity() const override;
    Function(FunctionDeclStmt* declaration, std::shared_ptr<Environment> closure) : declaration(declaration), closure(closure) {}
    LiteralValue call(Interpreter* interpreter, std::vector<LiteralValue> params) override;
    std::string toString() const override;
};