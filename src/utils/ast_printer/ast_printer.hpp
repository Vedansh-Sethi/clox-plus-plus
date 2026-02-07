#include <iostream>
#include "expression/expr.hpp"

class ASTPrinter : public ExprVisitor
{
private:
    std::string result;
    void visitGroupingExpr(GroupingExpr *expr);
    void visitBinaryExpr(BinaryExpr *expr);
    void visitUnaryExpr(UnaryExpr *expr);
    void visitLiteralExpr(LiteralExpr *expr);
    static ASTPrinter *instance;
    ASTPrinter() {};

public:
    ASTPrinter(ASTPrinter &obj) = delete;
    std::string print(Expr *expr);
    static ASTPrinter *getInstance();
};