#pragma once
#include <iostream>
#include <vector>
#include "token/token.hpp"

class ExprVisitor;
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;
class MultiExpr;
class TernaryExpr;


class Expr {
public: 
     virtual void accept(ExprVisitor* visitor) = 0;
};

class ExprVisitor {

public:
     virtual void visitBinaryExpr(BinaryExpr* Expr) = 0;
     virtual void visitGroupingExpr(GroupingExpr* Expr) = 0;
     virtual void visitLiteralExpr(LiteralExpr* Expr) = 0;
     virtual void visitUnaryExpr(UnaryExpr* Expr) = 0;
     virtual void visitMultiExpr(MultiExpr* Expr) = 0;
     virtual void visitTernaryExpr(TernaryExpr* Expr) = 0;

};

class BinaryExpr : public Expr {

public: 
     Expr* left;
     Token op;
     Expr* right;

     BinaryExpr(Expr* left,Token op,Expr* right) : left(left), op(op), right(right) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitBinaryExpr(this);
     }
 };

class GroupingExpr : public Expr {

public: 
     Expr* expression;

     GroupingExpr(Expr* expression) : expression(expression) {}

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
     Expr* right;

     UnaryExpr(Token op,Expr* right) : op(op), right(right) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitUnaryExpr(this);
     }
 };

class MultiExpr : public Expr {

public: 
     std::vector<Expr*> exprs;

     MultiExpr(std::vector<Expr*> exprs) : exprs(exprs) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitMultiExpr(this);
     }
 };

class TernaryExpr : public Expr {

public: 
     Expr* condition;
     Expr* ifTrue;
     Expr* ifFalse;

     TernaryExpr(Expr* condition,Expr* ifTrue,Expr* ifFalse = nullptr) : condition(condition), ifTrue(ifTrue), ifFalse(ifFalse) {}

     void accept(ExprVisitor* visitor) override {
         visitor->visitTernaryExpr(this);
     }
 };

