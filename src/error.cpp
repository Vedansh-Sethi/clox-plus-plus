#include <iostream>
#include "error.hpp"

ErrorHandler* ErrorHandler::instance = nullptr;

ErrorHandler* ErrorHandler::getInstance()
{
    if(instance == nullptr)
    {
        instance = new ErrorHandler();
        return instance;
    } else
    {
        return instance;
    }
}

void ErrorHandler::report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

void ErrorHandler::error(int line, std::string message)
{
    report(line, "", message);
}

void ErrorHandler::error(Token token, std::string message)
{
    if(token.type == TokenType::END_FILE)
    {
        report(token.line, " at end", message);
    } else
    {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}