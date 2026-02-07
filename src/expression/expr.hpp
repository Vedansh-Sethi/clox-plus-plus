#include <iostream>
#include "token/token.hpp"

class ExprVisitor;
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;


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

