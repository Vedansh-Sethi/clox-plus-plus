#include <iostream>
#include "token/token.hpp"

class Expr {};

class Binary : public Expr {

     const Expr* left;
     const Token op;
     const Expr* right;

     Binary(Expr* left,Token op,Expr* right) : left(left), op(op), right(right) {}

 };

class Grouping : public Expr {

     const Expr* expression;

     Grouping(Expr* expression) : expression(expression) {}

 };

class Literal : public Expr {

     const LiteralValue value;

     Literal(LiteralValue value) : value(value) {}

 };

class Unary : public Expr {

     const Token op;
     const Expr* right;

     Unary(Token op,Expr* right) : op(op), right(right) {}

 };

