#pragma once
#include <iostream>
#include <vector>
#include "token/token.hpp"

class StmtVisitor;
class BlockStmt;
class ExprStmt;
class PrintStmt;
class VarDeclStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class BreakStmt;
class ContinueStmt;


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
     virtual void visitIfStmt(IfStmt* Stmt) = 0;
     virtual void visitWhileStmt(WhileStmt* Stmt) = 0;
     virtual void visitForStmt(ForStmt* Stmt) = 0;
     virtual void visitBreakStmt(BreakStmt* Stmt) = 0;
     virtual void visitContinueStmt(ContinueStmt* Stmt) = 0;

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

class IfStmt : public Stmt {

public: 
     Expr* condition;
     Stmt* trueStmt;
     Stmt* falseStmt;

     IfStmt(Expr* condition,Stmt* trueStmt,Stmt* falseStmt) : condition(condition), trueStmt(trueStmt), falseStmt(falseStmt) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitIfStmt(this);
     }
 };

class WhileStmt : public Stmt {

public: 
     Expr* condition;
     Stmt* task;

     WhileStmt(Expr* condition,Stmt* task) : condition(condition), task(task) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitWhileStmt(this);
     }
 };

class ForStmt : public Stmt {

public: 
     Stmt* initializer;
     Expr* condition;
     Expr* increment;
     Stmt* task;

     ForStmt(Stmt* initializer,Expr* condition,Expr* increment,Stmt* task) : initializer(initializer), condition(condition), increment(increment), task(task) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitForStmt(this);
     }
 };

class BreakStmt : public Stmt {

public: 
     Token keyword;

     BreakStmt(Token keyword) : keyword(keyword) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitBreakStmt(this);
     }
 };

class ContinueStmt : public Stmt {

public: 
     Token keyword;

     ContinueStmt(Token keyword) : keyword(keyword) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitContinueStmt(this);
     }
 };

