#include "function/setter/setter.hpp"
#include "property/property.hpp"
#include "instance/instance.hpp"
#include "environment/environment.hpp"
#include "interpreter/interpreter.hpp"

int SetterFunction::arity()
{
    return 1;
}

std::string SetterFunction::toString() const
{
    return parentName.lexeme + " setter";
}

SetterFunction* SetterFunction::bind(Instance* instance)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return new SetterFunction(parentName, param, environment, body);
}

LiteralValue SetterFunction::call(Interpreter* interpreter, std::vector<LiteralValue> argument)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    environment->define(param.lexeme, argument[0]);
    try
    {
        interpreter->executeBlock(body, environment);
    }
    catch(Interpreter::ReturnInstruction returnInstruction)
    {
        return std::monostate();
    }
    return std::monostate();
}