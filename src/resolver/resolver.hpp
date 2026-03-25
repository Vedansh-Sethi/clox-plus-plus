#pragma once
#include <stack>
#include <unordered_map>
#include "expression/expr.hpp"
#include "statement/stmt.hpp"
#include "interpreter/interpreter.hpp"

enum FunctionType
{
    NONE, 
    FUNCTION,
    INITIALIZER,
    METHOD,
    PROPERTY,
};

enum class ClassType
{
    NONE,
    CLASS,
};

using Scope = std::unordered_map<std::string, bool>;

class Resolver : public StmtVisitor, public ExprVisitor
{
    // private variables
    Interpreter* interpreter;
    std::vector<Scope> scopes;
    FunctionType currentFunc = FunctionType::NONE;
    ClassType currentClass = ClassType::NONE;

    // statement visit methods
    void visitBlockStmt(BlockStmt *stmt) override;
    void visitVarDeclStmt(VarDeclStmt *stmt) override;
    void visitFunctionDeclStmt(FunctionDeclStmt *stmt) override;
    void visitIfStmt(IfStmt *stmt) override;
    void visitPrintStmt(PrintStmt *stmt) override;
    void visitReturnStmt(ReturnStmt* stmt) override;
    void visitWhileStmt(WhileStmt *stmt) override;
    void visitExprStmt(ExprStmt *stmt) override;
    void visitForStmt(ForStmt* stmt) override;
    void visitContinueStmt(ContinueStmt* stmt) override;
    void visitBreakStmt(BreakStmt* stmt) override;
    void visitClassDeclStmt(ClassDeclStmt* stmt) override;
    void visitPropertyStmt(PropertyStmt* stmt) override;
    void visitSetterDeclStmt(SetterDeclStmt* stmt) override;
    void visitGetterDeclStmt(GetterDeclStmt* stmt) override;

    // expression visit methods
    void visitVariableExpr(VariableExpr *expr) override;
    void visitAssignExpr(AssignExpr *expr) override;
    void visitBinaryExpr(BinaryExpr* Expr) override;
    void visitGroupingExpr(GroupingExpr* Expr) override;
    void visitLiteralExpr(LiteralExpr* expr) override;
    void visitUnaryExpr(UnaryExpr* expr) override;
    void visitCallExpr(CallExpr* expr) override;
    void visitLambdaExpr(LambdaExpr* expr) override;
    void visitTernaryExpr(TernaryExpr* expr) override;
    void visitMultiExpr(MultiExpr* expr) override;
    void visitLogicalExpr(LogicalExpr* expr) override;
    void visitGetExpr(GetExpr* expr) override;
    void visitSetExpr(SetExpr* expr) override;
    void visitThisExpr(ThisExpr* expr) override;
    

    // helper methods
    void resolve(const std::unique_ptr<Stmt> &stmts);
    void resolve(const std::unique_ptr<Expr> &expr);
    void resolve(const std::vector<std::unique_ptr<Expr>> &exprs);
    void resolveLocal(Expr *expr, Token name);
    void resolveFunction(FunctionDeclStmt* stmt, FunctionType type);
    void resolveProperty(PropertyStmt* stmt);
    void resolveGetter(GetterDeclStmt* stmt);
    void resolveSetter(SetterDeclStmt* stmt);

    void declare(Token name);
    void define(Token name);

    void beginScope();
    void endScope();

public:
    Resolver(Interpreter* interpreter) : interpreter(interpreter) {}
    void resolve(const std::vector<std::unique_ptr<Stmt>> &stmts);
};