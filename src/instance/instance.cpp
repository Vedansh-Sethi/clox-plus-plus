#include "instance/instance.hpp"
#include "error.hpp"
#include "function/function.hpp"


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

    std::shared_ptr<Function> method = klass->findMethod(name.lexeme);
    if(method != nullptr) return method->bind(shared_from_this());

    throw ErrorHandler::RuntimeError(name, "Undefined property " + name.lexeme);
}

void Instance::set(Token name, LiteralValue value)
{
    fields[name.lexeme] = value;
}