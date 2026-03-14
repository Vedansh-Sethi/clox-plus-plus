#pragma once
#include <iostream>
#include <unordered_map>
#include "token/token.hpp"

class Environment
{
private:
    std::unordered_map<std::string, LiteralValue> values;

public:
    std::shared_ptr<Environment> enclosing;
    void define(std::string ident, LiteralValue value);
    void assign(Token ident, LiteralValue value);
    LiteralValue get(Token ident);
    Environment() : enclosing(nullptr)
    {}
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing)
    {}
};