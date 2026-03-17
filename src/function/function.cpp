#include <vector>
#include <memory>
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "callable/callable.hpp"
#include "function/function.hpp"

LiteralValue Function::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for(int i = 0; i < params.size(); i++)
    {
        environment->define(params[i].lexeme, arguments[i]);
    }

    interpreter->executeBlock(body, environment);
    return std::monostate();
}

std::string Function::toString() const
{
    return "<fn " + name.lexeme + ">";
}

int Function::arity() const
{
    return params.size();
}