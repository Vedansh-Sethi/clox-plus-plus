#pragma once
#include <iostream>
#include <vector>
#include "expression/expr.hpp"

class StmtVisitor;
class ExprStmt;
class PrintStmt;


class Stmt {
public: 
     virtual void accept(StmtVisitor* visitor) = 0;
};

class StmtVisitor {

public:
     virtual void visitExprStmt(ExprStmt* Stmt) = 0;
     virtual void visitPrintStmt(PrintStmt* Stmt) = 0;

};

class ExprStmt : public Stmt {

public: 
     Expr* expr;

     ExprStmt(Expr* expr) : expr(expr) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitExprStmt(this);
     }
 };

class PrintStmt : public Stmt {

public: 
     Expr* expr;

     PrintStmt(Expr* expr) : expr(expr) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitPrintStmt(this);
     }
 };

