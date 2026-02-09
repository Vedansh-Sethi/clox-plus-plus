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
    ErrorHandler(ErrorHandler &obj) = delete;
    static void report(int line, std::string where, std::string message);
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
};