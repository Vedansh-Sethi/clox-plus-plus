#include "resolver/resolver.hpp"
#include "error.hpp"

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>> &stmts)
{
    for (const std::unique_ptr<Stmt> &stmt : stmts)
    {
        resolve(stmt);
    }
}

void Resolver::resolve(const std::unique_ptr<Stmt> &stmt)
{
    stmt->accept(this);
}

void Resolver::resolve(const std::unique_ptr<Expr> &expr)
{
    expr->accept(this);
}

void Resolver::beginScope()
{
    scopes.push_back(Scope());
}

void Resolver::endScope()
{
    scopes.pop_back();
}

void Resolver::visitBlockStmt(BlockStmt *stmt)
{
    beginScope();
    resolve(stmt->stmts);
    endScope();
}

void Resolver::visitVarDeclStmt(VarDeclStmt *stmt)
{
    declare(stmt->ident);
    if (stmt->init.get() != nullptr)
    {
        resolve(stmt->init);
    }
    define(stmt->ident);
}

void Resolver::declare(Token name)
{
    if (scopes.empty())
        return;

    Scope &scope = scopes.back();

    if (scope.count(name.lexeme))
    {
        ErrorHandler::error(name, "Already a variable with the given name in this scope");
        return;
    }
    scope[name.lexeme] = false;
}

void Resolver::define(Token name)
{
    if (scopes.empty())
        return;
    Scope &scope = scopes.back();
    scope[name.lexeme] = true;
}

void Resolver::visitVariableExpr(VariableExpr *expr)
{
    if (scopes.empty())
        return;
    Scope &scope = scopes.back();
    if (scope.count(expr->ident.lexeme) && scope[expr->ident.lexeme] == false)
    {
        ErrorHandler::error(expr->ident, "Can't read local variable in its own initializer");
    }

    resolveLocal(expr, expr->ident);
}

void Resolver::visitAssignExpr(AssignExpr *expr)
{
    resolve(expr->value);
    resolveLocal(expr, expr->ident);
}

void Resolver::visitFunctionDeclStmt(FunctionDeclStmt *stmt)
{
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION);
}

void Resolver::resolveFunction(FunctionDeclStmt *stmt, FunctionType type)
{
    FunctionType enclosingFunc = currentFunc;
    currentFunc = type;
    beginScope();
    for (Token param : stmt->params)
    {
        declare(param);
        define(param);
    }
    resolve(stmt->body);
    endScope();
    currentFunc = enclosingFunc;
}

void Resolver::visitExprStmt(ExprStmt *stmt)
{
    resolve(stmt->expr);
}

void Resolver::visitIfStmt(IfStmt *stmt)
{
    resolve(stmt->condition);
    resolve(stmt->trueStmt);
    if (stmt->falseStmt.get() != nullptr)
    {
        resolve(stmt->falseStmt);
    }
}

void Resolver::visitPrintStmt(PrintStmt *stmt)
{
    resolve(stmt->expr);
}

void Resolver::visitReturnStmt(ReturnStmt *stmt)
{
    if (currentFunc == NONE)
        ErrorHandler::error(stmt->keyword, "Can't return from global scope");
    resolve(stmt->value);
}

void Resolver::visitWhileStmt(WhileStmt *stmt)
{
    resolve(stmt->condition);
    resolve(stmt->task);
}

void Resolver::visitBinaryExpr(BinaryExpr *expr)
{
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitCallExpr(CallExpr *expr)
{
    resolve(expr->callee);
    resolve(expr->arguments);
}

void Resolver::visitGroupingExpr(GroupingExpr *expr)
{
    resolve(expr->expression);
}

void Resolver::visitLiteralExpr(LiteralExpr *expr) {}

void Resolver::visitLogicalExpr(LogicalExpr *expr)
{
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitUnaryExpr(UnaryExpr *expr)
{
    resolve(expr->right);
}

void Resolver::visitMultiExpr(MultiExpr *expr)
{
    resolve(expr->exprs);
}

void Resolver::visitTernaryExpr(TernaryExpr *expr)
{
    resolve(expr->condition);
    resolve(expr->ifTrue);
    if (expr->ifFalse.get() != nullptr)
    {
        resolve(expr->ifFalse);
    }
}

void Resolver::visitLambdaExpr(LambdaExpr *expr)
{
    FunctionType enclosingFunc = currentFunc;
    currentFunc = FUNCTION;
    beginScope();
    for(Token param : expr->params)
    {
        declare(param);
        define(param);
    }
    resolve(expr->body);
    endScope();
    currentFunc = enclosingFunc;
}

void Resolver::visitForStmt(ForStmt *stmt)
{
    beginScope();
    if (stmt->initializer.get() != nullptr)
        resolve(stmt->initializer);
    if (stmt->condition.get() != nullptr)
        resolve(stmt->condition);
    if (stmt->increment.get() != nullptr)
        resolve(stmt->increment);
    resolve(stmt->task);
    endScope();
}

void Resolver::visitContinueStmt(ContinueStmt *stmt) {}
void Resolver::visitBreakStmt(BreakStmt *stmt) {}

void Resolver::resolve(const std::vector<std::unique_ptr<Expr>> &exprs)
{
    for (const std::unique_ptr<Expr> &expr : exprs)
    {
        resolve(expr);
    }
}

void Resolver::resolveLocal(Expr *expr, Token name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        if (scopes[i].find(name.lexeme) != scopes[i].end())
        {
            interpreter->resolve(expr, scopes.size() - i - 1);
        }
    }
}