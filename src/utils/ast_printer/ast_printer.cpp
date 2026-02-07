#include <iostream>
#include <sysexits.h>
#include <fstream>
#include "utils/ast_printer/ast_printer.hpp"
#include "token/token.hpp"
#include "scanner/scanner.hpp"

ASTPrinter* ASTPrinter::instance = nullptr;

ASTPrinter* ASTPrinter::getInstance()
{
    if(instance == nullptr)
    {
        instance = new ASTPrinter();
        return instance;
    }
    else return instance;
}

std::string ASTPrinter::print(Expr *expr)
{
    expr->accept(this);
    return result;
}

void ASTPrinter::visitBinaryExpr(BinaryExpr *expr)
{
    std::string leftStr, rightStr;

    expr->left->accept(this);
    leftStr = result;

    expr->right->accept(this);
    rightStr = result;

    result = "(" + expr->op.lexeme + " " + leftStr + " " + rightStr + ")";
}

void ASTPrinter::visitGroupingExpr(GroupingExpr *expr)
{
    std::string groupStr;

    expr->expression->accept(this);
    groupStr = result;

    result = "(group " + groupStr + ")";
}

void ASTPrinter::visitLiteralExpr(LiteralExpr *expr)
{
    if (std::holds_alternative<std::monostate>(expr->value))
        result = "nil";
    result = literalToString(expr->value);
}

void ASTPrinter::visitUnaryExpr(UnaryExpr *expr)
{
    std::string unaryStr;

    expr->right->accept(this);
    unaryStr = result;

    result = "(" + expr->op.lexeme + " " + unaryStr + ")";
}