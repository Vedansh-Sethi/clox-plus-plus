#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include "token/token.hpp"
#include "expression/expr.hpp"
#include "statement/stmt.hpp"

class Parser
{
private:
    // private variables
    const std::vector<Token> tokens;
    int current = 0;

private:
    // expression parsing functions
    Expr *expression();
    Expr *commaSeparatedExpressions();
    Expr *ternary();
    Expr *assignment();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();

    // statement parsing functions
    Stmt *declaration();
    Stmt *varDeclStmt();
    Stmt *statement();
    Stmt *printStmt();
    Stmt *exprStmt();

public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {}
    std::vector<Stmt *> parse();

private:
    // helper functions
    inline bool isAtEnd() const;
    inline bool check(TokenType type) const;
    inline bool match();
    template <typename First, typename... Rest>
    bool match(First first, Rest... rest);
    inline Token advance();
    inline Token previous();
    inline Token consume(TokenType type, std::string message);
    inline Token peek(int ahead = 0) const;
    inline void addExpr(Expr *expr);

private:
    class ParseError : public std::runtime_error
    {
    public:
        ParseError() : std::runtime_error("Parse Error") {}
    };
    Parser::ParseError error(Token token, std::string message);
    void synchronize();
};
