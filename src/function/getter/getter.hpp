#pragma once
#include "callable/callable.hpp"
#include "statement/stmt.hpp"

class Property;
class Environment;
class Instance;

class GetterFunction : public Callable
{
    Token parentName;
    std::shared_ptr<Environment> closure;
    int arity() override;
    std::string toString() const override;
    const std::vector<std::unique_ptr<Stmt>> &body;

    public:
    GetterFunction(Token parentName, std::shared_ptr<Environment> closure, const std::vector<std::unique_ptr<Stmt>> &body) : closure(closure), body(body), parentName(parentName) {}
    GetterFunction* bind(Instance* instance);
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> arguments = std::vector<LiteralValue>(0)) override;
};