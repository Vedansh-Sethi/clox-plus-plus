#pragma once
#include <iostream>
#include "expression/expr.hpp"
#include "statement/stmt.hpp"
#include "environment/environment.hpp"
#include "callable/callable.hpp"
#include "token/token.hpp"
#include "function/native_function/nativeFunction.hpp"

using Map = std::unordered_map<Expr*, int>;

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    // private variables
    LiteralValue result;
    LiteralValue evaluate(Expr *expr);
    Map locals;
    void execute(Stmt *stmt);

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
    void visitLambdaExpr(LambdaExpr *expr) override;

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
    void visitFunctionDeclStmt(FunctionDeclStmt *stmt) override;
    void visitReturnStmt(ReturnStmt *stmt) override;

public:
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment;
    Interpreter();
    void interpret(const std::vector<std::unique_ptr<Stmt>> &stmts);
    void executeBlock(const std::vector<std::unique_ptr<Stmt>> &stmts, std::shared_ptr<Environment> enclosed);
    void resolve(Expr* expr, int depth);

private:
    // helper functions
    bool isTruthy(LiteralValue value);
    LiteralValue lookUpVariable(Token ident, Expr* expr);
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
    class ReturnInstruction
    {
    public:
        LiteralValue value;
        ReturnInstruction(LiteralValue value = std::monostate()) : value(value)
        {
        }
    };
    struct EnvironmentStorage
    {
        Interpreter *interpreter;
        std::shared_ptr<Environment> previous;
        ~EnvironmentStorage() { interpreter->environment = previous; }
    };
};
