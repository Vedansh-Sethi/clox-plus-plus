#pragma once
#include <iostream>
#include <vector>
#include "token/token.hpp"
#include "expression/expr.hpp"

class StmtVisitor;
class BlockStmt;
class ExprStmt;
class PrintStmt;
class VarDeclStmt;


class Stmt {
public: 
     virtual void accept(StmtVisitor* visitor) = 0;
};

class StmtVisitor {

public:
     virtual void visitBlockStmt(BlockStmt* Stmt) = 0;
     virtual void visitExprStmt(ExprStmt* Stmt) = 0;
     virtual void visitPrintStmt(PrintStmt* Stmt) = 0;
     virtual void visitVarDeclStmt(VarDeclStmt* Stmt) = 0;

};

class BlockStmt : public Stmt {

public: 
     std::vector<Stmt*> stmts;

     BlockStmt(std::vector<Stmt*> stmts) : stmts(stmts) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitBlockStmt(this);
     }
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

class VarDeclStmt : public Stmt {

public: 
     Token ident;
     Expr* init;

     VarDeclStmt(Token ident,Expr* init) : ident(ident), init(init) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitVarDeclStmt(this);
     }
 };

