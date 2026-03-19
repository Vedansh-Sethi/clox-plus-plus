#include <vector>
#include <memory>
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "callable/callable.hpp"
#include "function/function.hpp"
#include "environment/environment.hpp"
#include "statement/stmt.hpp"

LiteralValue Function::call(Interpreter *interpreter, std::vector<LiteralValue> arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (int i = 0; i < params.size(); i++)
    {
        environment->define(params[i].lexeme, arguments[i]);
    }

    try
    {
        interpreter->executeBlock(body, environment);
    }
    catch (Interpreter::ReturnInstruction returnInstruction)
    {
        if (this->isInitializer)
        {
            return closure->getAt(0, "this");
        }
        return returnInstruction.value;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    if (isInitializer)
        return closure->getAt(0, "this");
    return std::monostate();
}

std::shared_ptr<Function> Function::bind(std::shared_ptr<Instance> instance)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return std::make_shared<Function>(name, params, body, environment, isInitializer);
}

std::string Function::toString() const
{
    return "<fn " + name.lexeme + ">";
}

int Function::arity()
{
    return params.size();
}