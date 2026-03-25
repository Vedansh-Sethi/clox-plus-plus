#pragma once
#include "token/token.hpp"
#include <vector>

class GetterFunction;
class SetterFunction;
class Interpreter;
class Instance;

class Property
{
    std::shared_ptr<Environment> closure;

public:
    Property(Token name, GetterFunction *getter, SetterFunction *setter) : name(name), getter(getter), setter(setter) {}
    Token name;
    GetterFunction *getter;
    SetterFunction *setter;
};