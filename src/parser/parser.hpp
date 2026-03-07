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
    int loopCount = 0;

private:
    // expression parsing functions
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> commaSeparatedExpressions();
    std::unique_ptr<Expr> ternary();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> logical_or();
    std::unique_ptr<Expr> logical_and();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> primary();

    // statement parsing functions
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclStmt();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStmt();
    std::unique_ptr<Stmt> exprStmt();
    std::unique_ptr<Stmt> blockStmt();
    std::unique_ptr<Stmt> ifStmt();
    std::unique_ptr<Stmt> whileStmt();
    std::unique_ptr<Stmt> forStmt();
    std::unique_ptr<Stmt> breakStmt();
    std::unique_ptr<Stmt> continueStmt();

public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {}
    std::vector<std::unique_ptr<Stmt> > parse();

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
    inline void addExpr(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> finishCall(const std::unique_ptr<Expr>& callee);

private:
    class ParseError : public std::runtime_error
    {
    public:
        ParseError() : std::runtime_error("Parse Error") {}
    };
    Parser::ParseError error(Token token, std::string message);
    void synchronize();
};
