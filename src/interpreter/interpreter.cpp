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

void Interpreter::execute(Stmt* stmt)
{
    stmt->accept(this);
}

bool Interpreter::isTruthy(LiteralValue value)
{
    if (std::holds_alternative<std::monostate>(value))
        return false;
    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value);
    if(std::holds_alternative<double>(value))
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
    evaluate(expr->expression);
}

void Interpreter::visitUnaryExpr(UnaryExpr *expr)
{
    LiteralValue right = evaluate(expr->right);
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
    LiteralValue left = evaluate(expr->left);
    LiteralValue right = evaluate(expr->right);

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
    for(Expr* expression: expr->exprs)
    {
        values.push_back(evaluate(expression));
    }

    result = values.back();
}

void Interpreter::visitTernaryExpr(TernaryExpr *expr)
{
    LiteralValue condition = evaluate(expr->condition);
    LiteralValue ifTrue = evaluate(expr->ifTrue);
    LiteralValue ifFalse = evaluate(expr->ifFalse);

    result = isTruthy(condition) ? ifTrue : ifFalse;
}

void Interpreter::visitVariableExpr(VariableExpr *expr)
{
    result = environment.get(expr->ident);
}

void Interpreter::visitExprStmt(ExprStmt* stmt)
{
    evaluate(stmt->expr);
}

void Interpreter::visitPrintStmt(PrintStmt* stmt)
{
    LiteralValue value = evaluate(stmt->expr);
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitVarDeclStmt(VarDeclStmt* stmt)
{
    LiteralValue value = std::monostate();
    if(stmt->init != nullptr)
    {
        value = evaluate(stmt->init);
    }

    environment.define(stmt->ident.lexeme, value);
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

void Interpreter::interpret(std::vector<Stmt*> stmts)
{
    try 
    {
        for(Stmt* stmt : stmts)
        {
            execute(stmt);
        }
    }
    catch(ErrorHandler::RuntimeError error)
    {
        ErrorHandler::runtimeError(error);
    }
}
