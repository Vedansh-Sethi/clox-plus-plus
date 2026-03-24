#include <vector>
#include "class/class.hpp"
#include "callable/callable.hpp"
#include "interpreter/interpreter.hpp"
#include "function/function.hpp"
#include "token/token.hpp"
#include "instance/instance.hpp"

std::string Class::toString() const
{
    return name;
}

int Class::arity()
{
    Function* initializer = findMethod("init");
    if(initializer != nullptr)
    {
        return initializer->arity();
    }
    else return 0;
}

LiteralValue Class::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    Instance* instance = new Instance(this);
    Function* initializer = findMethod("init");
    if(initializer != nullptr)
    {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

Function* Class::findMethod(std::string name)
{
    if(methods.count(name))
    {
        return methods.at(name);
    }

    return nullptr;
}