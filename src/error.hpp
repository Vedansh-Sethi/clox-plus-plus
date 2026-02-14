#pragma once
#include <iostream>
#include "token/token.hpp"
#include "interpreter/interpreter.hpp"

class ErrorHandler
{
private:
    ErrorHandler() {};
    static ErrorHandler *instance;
    static ErrorHandler *getInstance();

public:
    static bool hadError;
    static bool hadRuntimeError;
    ErrorHandler(ErrorHandler &obj) = delete;
    static void report(int line, std::string where, std::string message);
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(Interpreter::RuntimeError error);
};