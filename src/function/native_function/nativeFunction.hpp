#pragma once
#include "token/token.hpp"

class Interpreter;
class Callable;

class ClockCallable : public Callable
{
    int arity() override;
    LiteralValue call(Interpreter* interpreter, std::vector<LiteralValue> arguments) override;
    std::string toString() const override;
};