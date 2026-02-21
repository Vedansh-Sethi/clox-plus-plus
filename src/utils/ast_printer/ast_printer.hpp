#pragma once
#include <iostream>
#include "expression/expr.hpp"

class ASTPrinter : public ExprVisitor
{
private:
    std::string result;
    void visitGroupingExpr(GroupingExpr *expr) override;
    void visitBinaryExpr(BinaryExpr *expr) override;
    void visitUnaryExpr(UnaryExpr *expr) override;
    void visitLiteralExpr(LiteralExpr *expr) override;
    void visitMultiExpr(MultiExpr *expr) override;
    void visitTernaryExpr(TernaryExpr *expr) override;
    void visitVariableExpr(VariableExpr *expr) override; 
    static ASTPrinter *instance;
    ASTPrinter() {};

public:
    ASTPrinter(ASTPrinter &obj) = delete;
    std::string print(Expr *expr);
    static ASTPrinter *getInstance();
};