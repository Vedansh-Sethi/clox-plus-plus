#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "token/token.hpp"
#include "expression/expr.hpp"

class StmtVisitor;
class BlockStmt;
class ExprStmt;
class PrintStmt;
class ReturnStmt;
class VarDeclStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class BreakStmt;
class ContinueStmt;
class FunctionDeclStmt;
class GetterDeclStmt;
class SetterDeclStmt;
class PropertyStmt;
class ClassDeclStmt;


class Stmt {
public: 
     virtual void accept(StmtVisitor* visitor) = 0;
};

class StmtVisitor {

public:
     virtual void visitBlockStmt(BlockStmt* Stmt) = 0;
     virtual void visitExprStmt(ExprStmt* Stmt) = 0;
     virtual void visitPrintStmt(PrintStmt* Stmt) = 0;
     virtual void visitReturnStmt(ReturnStmt* Stmt) = 0;
     virtual void visitVarDeclStmt(VarDeclStmt* Stmt) = 0;
     virtual void visitIfStmt(IfStmt* Stmt) = 0;
     virtual void visitWhileStmt(WhileStmt* Stmt) = 0;
     virtual void visitForStmt(ForStmt* Stmt) = 0;
     virtual void visitBreakStmt(BreakStmt* Stmt) = 0;
     virtual void visitContinueStmt(ContinueStmt* Stmt) = 0;
     virtual void visitFunctionDeclStmt(FunctionDeclStmt* Stmt) = 0;
     virtual void visitGetterDeclStmt(GetterDeclStmt* Stmt) = 0;
     virtual void visitSetterDeclStmt(SetterDeclStmt* Stmt) = 0;
     virtual void visitPropertyStmt(PropertyStmt* Stmt) = 0;
     virtual void visitClassDeclStmt(ClassDeclStmt* Stmt) = 0;

};

class BlockStmt : public Stmt {

public: 
     std::vector<std::unique_ptr<Stmt>> stmts;

     BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts) : stmts(std::move(stmts)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitBlockStmt(this);
     }
 };

class ExprStmt : public Stmt {

public: 
     std::unique_ptr<Expr> expr;

     ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitExprStmt(this);
     }
 };

class PrintStmt : public Stmt {

public: 
     std::unique_ptr<Expr> expr;

     PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitPrintStmt(this);
     }
 };

class ReturnStmt : public Stmt {

public: 
     Token keyword;
     std::unique_ptr<Expr> value;

     ReturnStmt(Token keyword,std::unique_ptr<Expr> value) : keyword(keyword), value(std::move(value)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitReturnStmt(this);
     }
 };

class VarDeclStmt : public Stmt {

public: 
     Token ident;
     std::unique_ptr<Expr> init;

     VarDeclStmt(Token ident,std::unique_ptr<Expr> init) : ident(ident), init(std::move(init)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitVarDeclStmt(this);
     }
 };

class IfStmt : public Stmt {

public: 
     std::unique_ptr<Expr> condition;
     std::unique_ptr<Stmt> trueStmt;
     std::unique_ptr<Stmt> falseStmt;

     IfStmt(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> trueStmt,std::unique_ptr<Stmt> falseStmt) : condition(std::move(condition)), trueStmt(std::move(trueStmt)), falseStmt(std::move(falseStmt)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitIfStmt(this);
     }
 };

class WhileStmt : public Stmt {

public: 
     std::unique_ptr<Expr> condition;
     std::unique_ptr<Stmt> task;

     WhileStmt(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> task) : condition(std::move(condition)), task(std::move(task)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitWhileStmt(this);
     }
 };

class ForStmt : public Stmt {

public: 
     std::unique_ptr<Stmt> initializer;
     std::unique_ptr<Expr> condition;
     std::unique_ptr<Expr> increment;
     std::unique_ptr<Stmt> task;

     ForStmt(std::unique_ptr<Stmt> initializer,std::unique_ptr<Expr> condition,std::unique_ptr<Expr> increment,std::unique_ptr<Stmt> task) : initializer(std::move(initializer)), condition(std::move(condition)), increment(std::move(increment)), task(std::move(task)) {}

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

class FunctionDeclStmt : public Stmt {

public: 
     Token name;
     std::vector<Token> params;
     std::vector<std::unique_ptr<Stmt>> body;

     FunctionDeclStmt(Token name,std::vector<Token> params,std::vector<std::unique_ptr<Stmt>> body) : name(name), params(std::move(params)), body(std::move(body)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitFunctionDeclStmt(this);
     }
 };

class GetterDeclStmt : public Stmt {

public: 
     Token keyword;
     std::vector<std::unique_ptr<Stmt>> body;

     GetterDeclStmt(Token keyword,std::vector<std::unique_ptr<Stmt>> body) : keyword(keyword), body(std::move(body)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitGetterDeclStmt(this);
     }
 };

class SetterDeclStmt : public Stmt {

public: 
     Token keyword;
     Token param;
     std::vector<std::unique_ptr<Stmt>> body;

     SetterDeclStmt(Token keyword,Token param,std::vector<std::unique_ptr<Stmt>> body) : keyword(keyword), param(param), body(std::move(body)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitSetterDeclStmt(this);
     }
 };

class PropertyStmt : public Stmt {

public: 
     Token name;
     std::unique_ptr<GetterDeclStmt> getter;
     std::unique_ptr<SetterDeclStmt> setter;

     PropertyStmt(Token name,std::unique_ptr<GetterDeclStmt> getter,std::unique_ptr<SetterDeclStmt> setter) : name(name), getter(std::move(getter)), setter(std::move(setter)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitPropertyStmt(this);
     }
 };

class ClassDeclStmt : public Stmt {

public: 
     Token name;
     std::vector<std::unique_ptr<FunctionDeclStmt>> methods;
     std::vector<std::unique_ptr<FunctionDeclStmt>> statics;
     std::vector<std::unique_ptr<PropertyStmt>> properties;

     ClassDeclStmt(Token name,std::vector<std::unique_ptr<FunctionDeclStmt>> methods,std::vector<std::unique_ptr<FunctionDeclStmt>> statics,std::vector<std::unique_ptr<PropertyStmt>> properties) : name(name), methods(std::move(methods)), statics(std::move(statics)), properties(std::move(properties)) {}

     void accept(StmtVisitor* visitor) override {
         visitor->visitClassDeclStmt(this);
     }
 };

