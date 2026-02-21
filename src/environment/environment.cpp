#include <iostream>
#include "environment/environment.hpp"
#include "error.hpp"

void Environment::define(std::string ident, LiteralValue value)
{
    values.insert({ident, value});
}

LiteralValue Environment::get(Token ident)
{
    if(values.count(ident.lexeme))
    {
        return values[ident.lexeme];
    }

    throw ErrorHandler::RuntimeError(ident, "Undefined variable " + ident.lexeme + ".");
}