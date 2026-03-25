#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "callable/callable.hpp"
#include "instance/instance.hpp"

class Interpreter;
class Function;

using Methods = std::unordered_map<std::string, Function *>;
using Properties = std::unordered_map<std::string, Property *>;

class Class : public Callable, public Instance
{
    std::string name;
    Methods methods;
    Class *metaclass;
    Properties props;

public:
    Class(std::string name, Methods methods, Properties props, Class *metaClass = nullptr) : name(name), methods(methods), props(props), Instance(metaClass) {}
    LiteralValue call(Interpreter *interpreter, std::vector<LiteralValue> arguments) override;
    int arity() override;
    std::string toString() const override;
    Function *findMethod(std::string name);
    Property *findProperty(std::string name);
};