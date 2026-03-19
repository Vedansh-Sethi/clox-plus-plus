#pragma once
#include <iostream>
#include <functional>
#include <chrono>
#include "callable/callable.hpp"
#include "function/native_function/nativeFunction.hpp"

int ClockCallable::arity()
{
    return 0;   
}

LiteralValue ClockCallable::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration<double>(now).count();
}

std::string ClockCallable::toString() const
{
    return "<native fn>";
}