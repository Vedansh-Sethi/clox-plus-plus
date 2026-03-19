#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "token/token.hpp"

class ExprVisitor;
class AssignExpr;
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class MultiExpr;
class TernaryExpr;
class VariableExpr;
class LogicalExpr;
class CallExpr;
class GetExpr;
class SetExpr;
class ThisExpr;
class LambdaExpr;

class Stmt;

class Expr {
public: 
     virtual void accept(ExprVisitor* visitor) = 0;
};

class ExprVisitor {

public:
     virtual void visitAssignExpr(AssignExpr* Expr) = 0;
     virtual void visitBinaryExpr(BinaryExpr* Expr) = 0;
     virtual void visitGroupingExpr(GroupingExpr* Expr) = 0;
     virtual void visitLiteralExpr(LiteralExpr* Expr) = 0;
     virtual void visitUnaryExpr(UnaryExpr* Expr) = 0;
     virtual void visitMultiExpr(MultiExpr* Expr) = 0;
     virtual void visitTernaryExpr(TernaryExpr* Expr) = 0;
     virtual void visitVariableExpr(VariableExpr* Expr) = 0;
     virtual void visitLogicalExpr(LogicalExpr* Expr) = 0;
     virtual void visitCallExpr(CallExpr* Expr) = 0;
     virtual void visitGetExpr(GetExpr* Expr) = 0;
     virtual void visitSetExpr(SetExpr* Expr) = 0;
     virtual void visitThisExpr(ThisExpr* Expr) = 0;
     virtual void visitLambdaExpr(LambdaExpr* Expr) = 0;

};

class AssignExpr : public Expr {

public: 
     Token ident;
     std::unique_ptr<Expr> value;

     AssignExpr(Token ident,std::unique_ptr<Expr> value) : ident(ident), value(std::move(value)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitAssignExpr(this);
     }
 };

class BinaryExpr : public Expr {

public: 
     std::unique_ptr<Expr> left;
     Token op;
     std::unique_ptr<Expr> right;

     BinaryExpr(std::unique_ptr<Expr> left,Token op,std::unique_ptr<Expr> right) : left(std::move(left)), op(op), right(std::move(right)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitBinaryExpr(this);
     }
 };

class GroupingExpr : public Expr {

public: 
     std::unique_ptr<Expr> expression;

     GroupingExpr(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitGroupingExpr(this);
     }
 };

class LiteralExpr : public Expr {

public: 
     LiteralValue value;

     LiteralExpr(LiteralValue value) : value(value) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitLiteralExpr(this);
     }
 };

class UnaryExpr : public Expr {

public: 
     Token op;
     std::unique_ptr<Expr> right;

     UnaryExpr(Token op,std::unique_ptr<Expr> right) : op(op), right(std::move(right)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitUnaryExpr(this);
     }
 };

class MultiExpr : public Expr {

public: 
     std::vector<std::unique_ptr<Expr>> exprs;

     MultiExpr(std::vector<std::unique_ptr<Expr>> exprs) : exprs(std::move(exprs)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitMultiExpr(this);
     }
 };

class TernaryExpr : public Expr {

public: 
     std::unique_ptr<Expr> condition;
     std::unique_ptr<Expr> ifTrue;
     std::unique_ptr<Expr> ifFalse;

     TernaryExpr(std::unique_ptr<Expr> condition,std::unique_ptr<Expr> ifTrue,std::unique_ptr<Expr> ifFalse) : condition(std::move(condition)), ifTrue(std::move(ifTrue)), ifFalse(std::move(ifFalse)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitTernaryExpr(this);
     }
 };

class VariableExpr : public Expr {

public: 
     Token ident;

     VariableExpr(Token ident) : ident(ident) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitVariableExpr(this);
     }
 };

class LogicalExpr : public Expr {

public: 
     std::unique_ptr<Expr> left;
     Token op;
     std::unique_ptr<Expr> right;

     LogicalExpr(std::unique_ptr<Expr> left,Token op,std::unique_ptr<Expr> right) : left(std::move(left)), op(op), right(std::move(right)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitLogicalExpr(this);
     }
 };

class CallExpr : public Expr {

public: 
     std::unique_ptr<Expr> callee;
     Token paren;
     std::vector<std::unique_ptr<Expr>> arguments;

     CallExpr(std::unique_ptr<Expr> callee,Token paren,std::vector<std::unique_ptr<Expr>> arguments) : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitCallExpr(this);
     }
 };

class GetExpr : public Expr {

public: 
     std::unique_ptr<Expr> object;
     Token name;

     GetExpr(std::unique_ptr<Expr> object,Token name) : object(std::move(object)), name(name) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitGetExpr(this);
     }
 };

class SetExpr : public Expr {

public: 
     std::unique_ptr<Expr> object;
     Token name;
     std::unique_ptr<Expr> value;

     SetExpr(std::unique_ptr<Expr> object,Token name,std::unique_ptr<Expr> value) : object(std::move(object)), name(name), value(std::move(value)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitSetExpr(this);
     }
 };

class ThisExpr : public Expr {

public: 
     Token keyword;

     ThisExpr(Token keyword) : keyword(keyword) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitThisExpr(this);
     }
 };

class LambdaExpr : public Expr {

public: 
     Token fun;
     std::vector<Token> params;
     std::vector<std::unique_ptr<Stmt>> body;

     LambdaExpr(Token fun,std::vector<Token> params,std::vector<std::unique_ptr<Stmt>> body) : fun(fun), params(std::move(params)), body(std::move(body)) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitLambdaExpr(this);
     }
 };

