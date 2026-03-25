#pragma once
#include "callable/callable.hpp"
#include "statement/stmt.hpp"

class Property;
class Environment;
class Instance;

class SetterFunction : public Callable
{
    Token parentName;
    std::shared_ptr<Environment> closure;
    Token param;
    int arity() override;
    std::string toString() const override;
    
    const std::vector<std::unique_ptr<Stmt>> &body;

public:
    SetterFunction(Token parentName, Token param, std::shared_ptr<Environment> closure, const std::vector<std::unique_ptr<Stmt>> &body) : parentName(parentName), param(param), closure(closure), body(body) {}
    SetterFunction *bind(Instance *instance);
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> arguments = std::vector<LiteralValue>(0)) override;
};