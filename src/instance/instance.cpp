#include "instance/instance.hpp"
#include "error.hpp"
#include "function/function.hpp"
#include "class/class.hpp"
#include "property/property.hpp"
#include "function/getter/getter.hpp"
#include "function/setter/setter.hpp"

std::string Instance::toString() const
{
    return klass->toString() + " instance";
}

LiteralValue Instance::get(Token name)
{
    if(fields.count(name.lexeme))
    {
        return fields.at(name.lexeme);
    }

    Function* method = klass->findMethod(name.lexeme);
    if(method != nullptr) return method->bind(this);

    throw ErrorHandler::RuntimeError(name, "Undefined property " + name.lexeme);
}

void Instance::set(Token name, LiteralValue value)
{
    fields[name.lexeme] = value;
}

Property* Instance::getProperty(std::string name)
{
    Property* prop = klass->findProperty(name);
    return prop;
}