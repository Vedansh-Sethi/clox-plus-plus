#include <vector>
#include <memory>
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "callable/callable.hpp"
#include "function/function.hpp"

LiteralValue Function::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for(int i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }

    interpreter->executeBlock(declaration->body, environment);
    return std::monostate();
}

std::string Function::toString() const
{
    return "<fn " + declaration->name.lexeme + ">";
}

int Function::arity() const
{
    return declaration->params.size();
}