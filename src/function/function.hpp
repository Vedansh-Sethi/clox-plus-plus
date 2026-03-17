#pragma once
#include <vector>
#include <memory>
#include "statement/stmt.hpp"

class Callable;
class Interpreter;

class Function : public Callable
{
private:
    Token name;
    std::vector<Token> params;
    const std::vector<std::unique_ptr<Stmt>> &body;
    std::shared_ptr<Environment> closure;

public:
    int arity() const override;
    Function(Token name, std::vector<Token> params, const std::vector<std::unique_ptr<Stmt>> &body, std::shared_ptr<Environment> closure) : name(name), params(params), body(body), closure(closure) {}
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> params) override;
    std::string toString() const override;
};