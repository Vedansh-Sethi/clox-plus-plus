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

void Environment::assign(Token ident, LiteralValue value)
{
    if(values.count(ident.lexeme))
    {
        values[ident.lexeme] = value;
        return;
    }

    throw ErrorHandler::RuntimeError(ident, "Undefined Variable '" + ident.lexeme + "'.");
}