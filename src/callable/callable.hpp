#pragma once
#include <vector>
#include "token/token.hpp"

class Interpreter;

class Callable
{
public:
    virtual int arity() = 0;
    virtual LiteralValue call(Interpreter* interpreter, std::vector<LiteralValue> arguments) = 0;
    virtual std::string toString() const = 0;
};

