#pragma once
#include <iostream>
#include "expression/expr.hpp"

using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

class Interpreter : public ExprVisitor
{
private:
    LiteralValue result;
    LiteralValue evaluate(Expr *expr);
    bool isTruthy(LiteralValue value);
    bool checkOperandValidity(Token opToken, LiteralValue right, LiteralValue left);
    bool checkOperandValidity(Token opToken, LiteralValue operand);
    bool isEqual(LiteralValue left, LiteralValue right);
    void visitLiteralExpr(LiteralExpr *expr);
    void visitGroupingExpr(GroupingExpr *expr);
    void visitUnaryExpr(UnaryExpr *expr);
    void visitBinaryExpr(BinaryExpr *expr);
    void visitMultiExpr(MultiExpr *expr);
    void visitTernaryExpr(TernaryExpr *expr);

public:
    void interpret(Expr *expr);
    class RuntimeError : public std::runtime_error
    {

    public:
        const Token token;
        RuntimeError(Token token, std::string message) : std::runtime_error(message), token(token) {}
    };

private:
    std::string stringify(LiteralValue value);
};
