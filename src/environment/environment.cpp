#include <iostream>
#include "environment/environment.hpp"
#include "error.hpp"

void Environment::define(std::string ident, LiteralValue value)
{
    values[ident] = value;
}

LiteralValue Environment::get(Token ident)
{
    if (values.count(ident.lexeme))
    {
        return values[ident.lexeme];
    }

    if (enclosing != nullptr)
        return enclosing->get(ident);

    throw ErrorHandler::RuntimeError(ident, "Undefined variable " + ident.lexeme + ".");
}

void Environment::assign(Token ident, LiteralValue value)
{
    if (values.count(ident.lexeme))
    {
        values[ident.lexeme] = value;
        return;
    }

    if (enclosing != nullptr)
    {
        enclosing->assign(ident, value);
        return;
    }

    throw ErrorHandler::RuntimeError(ident, "Undefined Variable '" + ident.lexeme + "'.");
}

LiteralValue Environment::getAt(int distance, std::string name)
{
    return ancestor(distance)->values.at(name);
}

void Environment::assignAt(int distance, Token ident, LiteralValue value)
{
    ancestor(distance)->values.at(ident.lexeme) = value;
}

Environment* Environment::ancestor(int distance)
{
    Environment* env = this;
    for(int i = 0; i < distance; i++)
    {
        env = env->enclosing.get();
    }
    return env;
}
