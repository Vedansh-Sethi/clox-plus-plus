#include <iostream>
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "error.hpp"

using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

LiteralValue Interpreter::evaluate(Expr *expr)
{
    expr->accept(this);
    return result;
}

void Interpreter::execute(Stmt *stmt)
{
    stmt->accept(this);
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt>> stmts, Environment *enclosed)
{
    Environment *previous = this->environment;
    EnvironmentStorage storage{this, previous};
    this->environment = enclosed;
    for (const std::unique_ptr<Stmt>& stmt : stmts)
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
    {
        return std::get<double>(value);
    }
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
    result = environment->get(expr->ident);
}

void Interpreter::visitAssignExpr(AssignExpr *expr)
{
    LiteralValue value = evaluate((expr->value).get());
    result = value;
    environment->assign(expr->ident, value);
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
    if (stmt->init != nullptr)
    {
        value = evaluate((stmt->init).get());
    }

    environment->define(stmt->ident.lexeme, value);
}

void Interpreter::visitBlockStmt(BlockStmt *stmt)
{
    executeBlock(stmt->stmts, new Environment(environment));
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

void Interpreter::visitWhileStmt(WhileStmt *stmt)
{
    try
    {
        while (isTruthy(evaluate((stmt->condition).get())))
        {
            try
            {
                execute((stmt->task).get());
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
    Environment *previous = this->environment;
    EnvironmentStorage storage{this, previous};
    Environment *enclosed = new Environment(this->environment);
    this->environment = enclosed;
    if (stmt->initializer != nullptr)
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

std::string Interpreter::stringify(LiteralValue value)
{
    if (std::holds_alternative<std::monostate>(value))
    {
        return "nil";
    }
    if (std::holds_alternative<double>(value))
    {
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
}

void Interpreter::visitCallExpr(CallExpr *expr)
{
    LiteralValue callee = evaluate((expr->callee).get());

    std::vector<LiteralValue> arguments;
    for (const std::unique_ptr<Expr>& argument : expr->arguments)
    {
        arguments.push_back(evaluate(argument.get()));
    }
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>> stmts)
{
    try
    {
        for (const std::unique_ptr<Stmt>& stmt : stmts)
        {
            execute(stmt.get());
        }
    }
    catch (ErrorHandler::RuntimeError error)
    {
        ErrorHandler::runtimeError(error);
    }
}
