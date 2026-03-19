#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "callable/callable.hpp"

class Interpreter;
class Function;

using Methods = std::unordered_map<std::string, std::shared_ptr<Function>>;

class Class : public Callable, public std::enable_shared_from_this<Class>
{
    std::string name;
    Methods methods;

public:
    Class(std::string name, Methods methods) : name(name), methods(methods) {}
    LiteralValue call(Interpreter* interpreter, std::vector<LiteralValue> arguments) override;
    int arity() override;
    std::string toString() const override;
    std::shared_ptr<Function> findMethod(std::string name);
};