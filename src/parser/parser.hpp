#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include "token/token.hpp"
#include "expression/expr.hpp"

class Parser
{
private:
    const std::vector<Token> tokens;
    int current = 0;
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();

public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {}
    Expr* parse();

private:
    inline bool isAtEnd() const;
    inline bool check(TokenType type) const;
    inline bool match();
    template <typename First, typename... Rest>
    bool match(First first, Rest... rest);
    inline Token advance();
    inline Token previous();
    inline Token peek(int ahead = 0) const;
    inline void addExpr(Expr *expr);

private:
    class ParseError : public std::runtime_error
    {
    public:
        ParseError() : std::runtime_error("Parse Error") {}
    };
    Token consume(TokenType type, std::string message);
    Parser::ParseError error(Token token, std::string message);
    void synchronize();
};