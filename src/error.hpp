#pragma once
#include <iostream>
#include "token/token.hpp"

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
    class RuntimeError : public std::runtime_error
    {

    public:
        const Token token;
        RuntimeError(Token token, std::string message) : std::runtime_error(message), token(token) {}
    };
    static void report(int line, std::string where, std::string message);
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(RuntimeError error);
};