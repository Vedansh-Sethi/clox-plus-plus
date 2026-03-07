#pragma once
#include <iostream>
#include "expression/expr.hpp"
#include "statement/stmt.hpp"
#include "environment/environment.hpp"

using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    // private variables
    LiteralValue result;
    LiteralValue evaluate(Expr *expr);
    Environment *environment;
    void execute(Stmt *stmt);
    void executeBlock(std::vector<std::unique_ptr<Stmt>> stmts, Environment *enclosed);

private:
    // expression interpreting functions
    void visitLiteralExpr(LiteralExpr *expr) override;
    void visitGroupingExpr(GroupingExpr *expr) override;
    void visitUnaryExpr(UnaryExpr *expr) override;
    void visitBinaryExpr(BinaryExpr *expr) override;
    void visitMultiExpr(MultiExpr *expr) override;
    void visitTernaryExpr(TernaryExpr *expr) override;
    void visitVariableExpr(VariableExpr *expr) override;
    void visitAssignExpr(AssignExpr *expr) override;
    void visitLogicalExpr(LogicalExpr *expr) override;
    void visitCallExpr(CallExpr *expr) override;

    // statement interpreting functions
    void visitPrintStmt(PrintStmt *stmt) override;
    void visitExprStmt(ExprStmt *stmt) override;
    void visitVarDeclStmt(VarDeclStmt *stmt) override;
    void visitBlockStmt(BlockStmt *stmt) override;
    void visitIfStmt(IfStmt *stmt) override;
    void visitWhileStmt(WhileStmt *stmt) override;
    void visitBreakStmt(BreakStmt *stmt) override;
    void visitContinueStmt(ContinueStmt *stmt) override;
    void visitForStmt(ForStmt *stmt) override;

public:
    Interpreter() : environment(new Environment())
    {
    }
    void interpret(std::vector<Stmt *> stmts);

private:
    // helper functions
    bool isTruthy(LiteralValue value);
    bool checkOperandValidity(Token opToken, LiteralValue right, LiteralValue left);
    bool checkOperandValidity(Token opToken, LiteralValue operand);
    bool isEqual(LiteralValue left, LiteralValue right);
    std::string stringify(LiteralValue value);
    class BreakInstruction
    {
    };
    class ContinueInstruction
    {
    };
    struct EnvironmentStorage
    {
        Interpreter *interpreter;
        Environment *previous;
        ~EnvironmentStorage() { interpreter->environment = previous; }
    };
};
