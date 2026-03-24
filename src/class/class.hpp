#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "callable/callable.hpp"
#include "instance/instance.hpp"

class Interpreter;
class Function;

using Methods = std::unordered_map<std::string, Function *>;

class Class : public Callable, public Instance
{
    std::string name;
    Methods methods;
    Class *metaclass;

public:
    Class(std::string name, Methods methods, Class *metaClass = nullptr) : name(name), methods(methods), Instance(metaClass) {}
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> arguments) override;
    int arity() override;
    std::string toString() const override;
    Function *findMethod(std::string name);
};