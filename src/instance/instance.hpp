#pragma once
#include <unordered_map>
#include <string>
#include "token/token.hpp"

using Fields = std::unordered_map<std::string, LiteralValue>;

class Class;

class Instance
{
    Class* klass;
    Fields fields;

public:
    Instance(Class* klass) : klass(klass) {}
    LiteralValue get(Token name);
    void set(Token name, LiteralValue value);
    std::string toString() const;
};