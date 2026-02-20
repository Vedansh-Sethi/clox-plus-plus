#pragma once
#include <iostream>
#include "expression/expr.hpp"
#include "statement/stmt.hpp"

using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    // private variables
    LiteralValue result;
    LiteralValue evaluate(Expr *expr);
    void execute(Stmt *stmt);

private:
    // expression interpreting functions
    void visitLiteralExpr(LiteralExpr *expr) override;
    void visitGroupingExpr(GroupingExpr *expr) override;
    void visitUnaryExpr(UnaryExpr *expr) override;
    void visitBinaryExpr(BinaryExpr *expr) override;
    void visitMultiExpr(MultiExpr *expr) override;
    void visitTernaryExpr(TernaryExpr *expr) override;

    // statement interpreting functions
    void visitPrintStmt(PrintStmt* stmt) override;
    void visitExprStmt(ExprStmt* stmt) override;


public:
    void interpret(std::vector<Stmt*> stmts);
    class RuntimeError : public std::runtime_error
    {

    public:
        const Token token;
        RuntimeError(Token token, std::string message) : std::runtime_error(message), token(token) {}
    };

private:
    // helper functions
    bool isTruthy(LiteralValue value);
    bool checkOperandValidity(Token opToken, LiteralValue right, LiteralValue left);
    bool checkOperandValidity(Token opToken, LiteralValue operand);
    bool isEqual(LiteralValue left, LiteralValue right);
    std::string stringify(LiteralValue value);
};
