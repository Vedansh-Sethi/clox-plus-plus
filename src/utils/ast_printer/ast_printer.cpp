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

void ASTPrinter::visitMultiExpr(MultiExpr *expr)
{
    std::vector<std::string> exprStrings;
    for(Expr* unitExpr : expr->exprs)
    {
        unitExpr->accept(this);
        std::cout << "expression string added" << std::endl;
        exprStrings.push_back(result);
    }

    result = "(multi ";
    for(std::string exprString : exprStrings)
    {
        result += exprString + " ";
    }

    result += ")";
}

void ASTPrinter::visitTernaryExpr(TernaryExpr* expr)
{
    expr->condition->accept(this);
    std::string conditionString = result;

    expr->ifTrue->accept(this);
    std::string ifTrueString = result;

    std::string ifFalseString;
    if(expr->ifFalse != nullptr)
    {
        expr->ifFalse->accept(this);
        ifFalseString = result;
    }

    result = "(ternary condition : " + conditionString + " if true : " + ifTrueString;
    expr->ifFalse == nullptr ? result += ")" : result += " if false : " + ifFalseString + ")";
}