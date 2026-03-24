#pragma once
#include <vector>
#include <memory>
#include "callable/callable.hpp"

class Interpreter;
class Stmt;
class Environment;

class Function : public Callable
{
private:
    Token name;
    std::vector<Token> params;
    const std::vector<std::unique_ptr<Stmt>> &body;

public:
    int arity() override;
    Function(Token name, std::vector<Token> params, const std::vector<std::unique_ptr<Stmt>> &body, std::shared_ptr<Environment> closure, bool isInitializer) : name(name), params(params), body(body), closure(closure), isInitializer(isInitializer) {}
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> params) override;
    std::string toString() const override;
    Function* bind(Instance* instance);

    bool isInitializer = false;
    std::shared_ptr<Environment> closure;
};