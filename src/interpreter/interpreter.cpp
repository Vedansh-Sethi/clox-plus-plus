#include <iostream>
#include <cmath>
#include "interpreter/interpreter.hpp"
#include "error.hpp"
#include "callable/callable.hpp"
#include "function/function.hpp"
#include "function/native_function/nativeFunction.hpp"
#include "class/class.hpp"
#include "instance/instance.hpp"

using LiteralValue = std::variant<std::monostate, std::string, double, bool, Callable *, Instance *>;

bool checkInt(double n)
{
    if (n - floor(n) > 1e-6)
        return false;
    if (ceil(n) - n > 1e-6)
        return false;
    return true;
}

LiteralValue Interpreter::evaluate(Expr *expr)
{
    expr->accept(this);
    return result;
}

void Interpreter::execute(Stmt *stmt)
{
    stmt->accept(this);
}

void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>> &stmts, std::shared_ptr<Environment> enclosed)
{
    std::shared_ptr<Environment> previous = this->environment;
    EnvironmentStorage storage{this, previous};
    this->environment = enclosed;
    for (const std::unique_ptr<Stmt> &stmt : stmts)
    {
        execute(stmt.get());
    }
}

bool Interpreter::isTruthy(LiteralValue value)
{
    if (std::holds_alternative<std::monostate>(value))
        return false;
    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value);
    if (std::holds_alternative<double>(value))
        return std::get<double>(value);
    return true;
}

bool Interpreter::checkOperandValidity(Token opToken, LiteralValue operand)
{
    if (!std::holds_alternative<double>(operand))
    {
        throw ErrorHandler::RuntimeError(opToken, "Operands must be numbers");
        return false;
    }
    return true;
}

bool Interpreter::checkOperandValidity(Token opToken, LiteralValue right, LiteralValue left)
{
    if (!std::holds_alternative<double>(left) || !std::holds_alternative<double>(right))
    {
        throw ErrorHandler::RuntimeError(opToken, "Operands must be numbers");
        return false;
    }
    if (opToken.type == SLASH && std::get<double>(right) == 0)
    {
        throw ErrorHandler::RuntimeError(opToken, "Division by zero error");
        return false;
    }
    if (opToken.type == MODULUS)
    {
        if (!checkInt(std::get<double>(right)) || !checkInt(std::get<double>(left)))
        {
            throw ErrorHandler::RuntimeError(opToken, "Operands must be Integers");
            return false;
        }
        if (std::get<double>(right) == 0)
        {
            throw ErrorHandler::RuntimeError(opToken, "Remainder with respect to zero error");
            return false;
        }
    }
    return true;
}

bool Interpreter::isEqual(LiteralValue left, LiteralValue right)
{
    if (std::holds_alternative<std::monostate>(right) && std::holds_alternative<std::monostate>(left))
        return true;
    if (std::holds_alternative<std::monostate>(left))
        return false;
    return left == right;
}

void Interpreter::visitLiteralExpr(LiteralExpr *expr)
{
    result = expr->value;
}

void Interpreter::visitGroupingExpr(GroupingExpr *expr)
{
    evaluate((expr->expression).get());
}

void Interpreter::visitUnaryExpr(UnaryExpr *expr)
{
    LiteralValue right = evaluate((expr->right).get());
    switch (expr->op.type)
    {
    case BANG:
        result = !isTruthy(right);
        break;
    case MINUS:
        if (checkOperandValidity(expr->op, right))
        {
            result = -std::get<double>(right);
        }
        break;
    }
}

void Interpreter::visitBinaryExpr(BinaryExpr *expr)
{
    LiteralValue left = evaluate((expr->left).get());
    LiteralValue right = evaluate((expr->right).get());

    switch (expr->op.type)
    {
    case MINUS:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) - std::get<double>(right);
        break;
    case SLASH:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) / std::get<double>(right);
        break;
    case MODULUS:
        if (checkOperandValidity(expr->op, right, left))
            result = (double)((int)std::get<double>(left) % (int)std::get<double>(right));
        break;
    case STAR:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) * std::get<double>(right);
        break;
    case PLUS:
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
            result = std::get<double>(left) + std::get<double>(right);
        else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
            result = std::get<std::string>(left) + std::get<std::string>(right);
        else
            throw ErrorHandler::RuntimeError(expr->op, "operands must be two numbers or two strings");
        break;
    case GREATER:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) > std::get<double>(right);
        break;
    case GREATER_EQUAL:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) >= std::get<double>(right);
        break;
    case LESS:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) < std::get<double>(right);
        break;
    case LESS_EQUAL:
        if (checkOperandValidity(expr->op, right, left))
            result = std::get<double>(left) <= std::get<double>(right);
        break;
    case BANG_EQUAL:
        result = !isEqual(left, right);
        break;
    case EQUAL_EQUAL:
        result = isEqual(left, right);
        break;
    }
}

void Interpreter::visitMultiExpr(MultiExpr *expr)
{
    std::vector<LiteralValue> values;
    for (const std::unique_ptr<Expr> &expression : (expr->exprs))
    {
        values.push_back(evaluate(expression.get()));
    }

    if (!values.empty())
    {
        result = values.back();
    }
}

void Interpreter::visitTernaryExpr(TernaryExpr *expr)
{
    LiteralValue condition = evaluate((expr->condition).get());
    LiteralValue ifTrue = evaluate((expr->ifTrue).get());
    LiteralValue ifFalse = evaluate((expr->ifFalse).get());

    result = isTruthy(condition) ? ifTrue : ifFalse;
}

void Interpreter::visitVariableExpr(VariableExpr *expr)
{
    result = lookUpVariable(expr->ident, expr);
}

LiteralValue Interpreter::lookUpVariable(Token ident, Expr *expr)
{
    if (locals.count(expr))
    {
        int distance = locals[expr];
        return environment->getAt(distance, ident.lexeme);
    }
    else
        return globals->get(ident);
}

void Interpreter::visitAssignExpr(AssignExpr *expr)
{
    LiteralValue value = evaluate((expr->value).get());
    if (locals.count(expr))
    {
        int distance = locals[expr];
        environment->assignAt(distance, expr->ident, value);
    }
    else
        globals->assign(expr->ident, value);
}

void Interpreter::visitLogicalExpr(LogicalExpr *expr)
{
    LiteralValue left = evaluate((expr->left).get());

    if (expr->op.type == AND)
    {
        if (!isTruthy(left))
        {
            result = left;
            return;
        }
    }
    else
    {
        if (isTruthy(left))
        {
            result = left;
            return;
        }
    }

    result = evaluate((expr->right).get());
    return;
}

void Interpreter::visitReturnStmt(ReturnStmt *stmt)
{
    LiteralValue value = std::monostate();
    if (stmt->value.get() == nullptr)
    {
        throw ReturnInstruction(value);
    }
    value = evaluate(stmt->value.get());
    throw ReturnInstruction(value);
}

void Interpreter::visitLambdaExpr(LambdaExpr *expr)
{
    Token name = Token(IDENTIFIER, "anonymous", "anonymous", expr->fun.line);
    Function *lambda = new Function(name, expr->params, expr->body, this->environment, false);
    result = lambda;
}

void Interpreter::visitCallExpr(CallExpr *expr)
{
    LiteralValue callee = evaluate((expr->callee).get());
    if (!std::holds_alternative<Instance *>(callee) && !std::holds_alternative<Callable *>(callee))
    {
        throw ErrorHandler::RuntimeError(expr->paren, "Can only call functions and classes");
    }

    Callable *function;
    if (std::holds_alternative<Instance *>(callee))
    {
        function = dynamic_cast<Class *>(std::get<Instance *>(callee));
        if(function == nullptr) throw ErrorHandler::RuntimeError(expr->paren, "Can only call functions and classes");
    }

    if(std::holds_alternative<Callable *>(callee))
    {
        function = std::get<Callable*>(callee);
    }

    std::vector<LiteralValue> arguments;
    for (const std::unique_ptr<Expr> &argument : expr->arguments)
    {
        arguments.push_back(evaluate(argument.get()));
    }

    if (arguments.size() != function->arity())
    {
        throw ErrorHandler::RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + " arguments");
    }
    result = function->call(this, arguments);
}

void Interpreter::visitFunctionDeclStmt(FunctionDeclStmt *stmt)
{
    Function *func = new Function(stmt->name, stmt->params, stmt->body, this->environment, false);
    this->environment->define(stmt->name.lexeme, func);
}

void Interpreter::visitExprStmt(ExprStmt *stmt)
{
    evaluate((stmt->expr).get());
}

void Interpreter::visitPrintStmt(PrintStmt *stmt)
{
    LiteralValue value = evaluate((stmt->expr).get());
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitVarDeclStmt(VarDeclStmt *stmt)
{
    LiteralValue value = std::monostate();
    if (stmt->init.get() != nullptr)
    {
        value = evaluate(stmt->init.get());
    }
    environment->define(stmt->ident.lexeme, value);
}

void Interpreter::visitBlockStmt(BlockStmt *stmt)
{
    executeBlock(stmt->stmts, std::make_shared<Environment>(environment));
}

void Interpreter::visitIfStmt(IfStmt *stmt)
{
    if (isTruthy(evaluate((stmt->condition).get())))
    {
        execute((stmt->trueStmt).get());
    }
    else if (stmt->falseStmt != nullptr)
    {
        execute((stmt->falseStmt).get());
    }
}

void Interpreter::visitClassDeclStmt(ClassDeclStmt *stmt)
{
    environment->define(stmt->name.lexeme, std::monostate());
    std::unordered_map<std::string, Function *> methods;
    std::unordered_map<std::string, Function *> statics;

    for (const std::unique_ptr<FunctionDeclStmt> &method : stmt->methods)
    {
        Function *function = new Function(method->name, method->params, method->body, environment, method->name.lexeme == "init");
        methods[method->name.lexeme] = function;
    }

    for (const std::unique_ptr<FunctionDeclStmt> &stat : stmt->statics)
    {
        Function *func = new Function(stat->name, stat->params, stat->body, environment, false);
        statics[stat->name.lexeme] = func;
    }

    Class *metaClass = new Class(stmt->name.lexeme + " class", statics);
    Class *klass = new Class(stmt->name.lexeme, methods, metaClass);
    environment->assign(stmt->name, dynamic_cast<Instance *>(klass));
}

void Interpreter::visitThisExpr(ThisExpr *expr)
{
    result = lookUpVariable(expr->keyword, expr);
}

void Interpreter::visitWhileStmt(WhileStmt *stmt)
{
    try
    {
        while (isTruthy(evaluate(stmt->condition.get())))
        {
            try
            {
                execute(stmt->task.get());
            }
            catch (ContinueInstruction &continueInstruction)
            {
            }
        }
    }
    catch (BreakInstruction &breakInstruction)
    {
    }
}

void Interpreter::visitForStmt(ForStmt *stmt)
{
    std::shared_ptr<Environment> previous = this->environment;
    EnvironmentStorage storage{this, previous};
    std::shared_ptr<Environment> enclosed = std::make_shared<Environment>(previous);
    this->environment = enclosed;
    if (stmt->initializer.get() != nullptr)
    {
        execute((stmt->initializer).get());
    }
    try
    {
        while (isTruthy(evaluate((stmt->condition).get())))
        {
            try
            {
                execute((stmt->task).get());
            }
            catch (ContinueInstruction)
            {
            }
            if (stmt->increment != nullptr)
            {
                evaluate((stmt->increment).get());
            }
        }
    }
    catch (BreakInstruction)
    {
    }
}

void Interpreter::visitBreakStmt(BreakStmt *stmt)
{
    throw BreakInstruction();
}

void Interpreter::visitContinueStmt(ContinueStmt *stmt)
{
    throw ContinueInstruction();
}

void Interpreter::visitGetExpr(GetExpr *expr)
{
    LiteralValue object = evaluate(expr->object.get());
    if (std::holds_alternative<Instance *>(object))
    {
        result = (std::get<Instance *>(object))->get(expr->name);
        return;
    }
    throw ErrorHandler::RuntimeError(expr->name, "Only instances have properties");
}

void Interpreter::visitSetExpr(SetExpr *expr)
{
    LiteralValue object = evaluate(expr->object.get());
    if (!std::holds_alternative<Instance *>(object))
    {
        throw ErrorHandler::RuntimeError(expr->name, "Only class instances have fields");
    }

    LiteralValue value = evaluate(expr->value.get());
    Instance *obj = std::get<Instance *>(object);

    obj->set(expr->name, value);
}

std::string Interpreter::stringify(LiteralValue value)
{
    if (std::holds_alternative<std::monostate>(value))
    {
        return "nil";
    }
    if (std::holds_alternative<double>(value))
    {
        if (checkInt(std::get<double>(value)))
            return std::to_string((int)std::get<double>(value));
        return std::to_string(std::get<double>(value));
    }

    if (std::holds_alternative<bool>(value))
    {
        bool val = std::get<bool>(value);
        return val ? "true" : "false";
    }

    if (std::holds_alternative<std::string>(value))
    {
        return std::get<std::string>(value);
    }

    if (std::holds_alternative<Callable *>(value))
    {
        return std::get<Callable *>(value)->toString();
    }
    if (std::holds_alternative<Instance *>(value))
    {
        return std::get<Instance *>(value)->toString();
    }

    return "DEBUG";
}

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>> &stmts)
{
    try
    {
        for (const std::unique_ptr<Stmt> &stmt : stmts)
        {
            if (stmt.get() == nullptr)
            {
                std::cout << "THE ARGUMENTS IS NULL" << std::endl;
            }
            execute(stmt.get());
        }
    }
    catch (ErrorHandler::RuntimeError error)
    {
        ErrorHandler::runtimeError(error);
    }
}

void Interpreter::resolve(Expr *expr, int depth)
{
    locals[expr] = depth;
}

Interpreter::Interpreter() : environment(globals)
{
    Callable *clockCallable = new ClockCallable();
    globals->define("clock", clockCallable);
}