#include <vector>
#include "class/class.hpp"
#include "callable/callable.hpp"
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "instance/instance.hpp"

std::string Class::toString() const
{
    return name;
}

int Class::arity() const
{
    return 0;
}

LiteralValue Class::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(shared_from_this());
    return instance;
}

std::shared_ptr<Function> Class::findMethod(std::string name)
{
    if(methods.count(name))
    {
        return methods.at(name);
    }

    return nullptr;
}