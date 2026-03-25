#include "function/getter/getter.hpp"
#include "interpreter/interpreter.hpp"
#include "property/property.hpp"
#include "instance/instance.hpp"


std::string GetterFunction::toString() const
{
    return parentName.lexeme + " getter";
}

int GetterFunction::arity() 
{
    return 0;
}

GetterFunction* GetterFunction::bind(Instance* instance)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return new GetterFunction(parentName, environment, body);
}

LiteralValue GetterFunction::call(Interpreter* interpreter, std::vector<LiteralValue> arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    try
    {
        interpreter->executeBlock(body, environment);
    }
    catch(Interpreter::ReturnInstruction returnInstruction)
    {
        return returnInstruction.value;
    }
    return std::monostate();
}