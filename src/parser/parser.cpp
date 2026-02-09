#include <iostream>
#include "parser/parser.hpp"
#include "token/token.hpp"
#include "error.hpp"

bool Parser::check(TokenType type) const
{
    if (isAtEnd())
        return false;
    return peek().type == type;
}

bool Parser::match()
{
    return false;
}

template <typename First, typename... Rest>
bool Parser::match(First first, Rest... rest)
{
    if (check(first))
    {
        advance();
        return true;
    }

    return match(rest...);
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();
    throw Parser::error(peek(), message);
}

void Parser::synchronize()
{
    advance();
    while (!isAtEnd())
    {
        if (previous().type == TokenType::SEMICOLON)
            return;
        switch (peek().type)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }
        advance();
    }
}

Parser::ParseError Parser::error(Token token, std::string message)
{
    ErrorHandler::error(token, message);
    return Parser::ParseError();
}

bool Parser::isAtEnd() const
{
    return peek().type == TokenType::END_FILE;
}

Token Parser::previous()
{
    return tokens[current - 1];
}

Token Parser::peek(int ahead) const
{
    if (current + ahead >= tokens.size())
    {
        return tokens.back();
    }
    return tokens[current + ahead];
};

Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

Expr *Parser::expression()
{
    return commaSeparatedExpressions();
}

Expr *Parser::commaSeparatedExpressions()
{
    Expr *expr = equality();

    if(!check(COMMA))
    {
        return expr;
    }

    std::vector<Expr*> exprs;
    exprs.push_back(expr);
    std::cout << "expression added" << std::endl;

    while(match(COMMA))
    {
        expr = equality();
        exprs.push_back(expr);
        std::cout << "expression added" << std::endl;
    }

    return new MultiExpr(exprs);
}

Expr *Parser::equality()
{
    Expr *expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token op = previous();
        Expr *right = comparison();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::comparison()
{
    Expr *expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token op = previous();
        Expr *right = term();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::term()
{
    Expr *expr = factor();

    while (match(PLUS, MINUS))
    {
        Token op = previous();
        Expr *right = factor();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::factor()
{
    Expr *expr = unary();

    while (match(SLASH, STAR))
    {
        Token op = previous();
        Expr *right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::unary()
{
    if (match(BANG, MINUS))
    {
        Token op = previous();
        Expr *right = unary();
        return new UnaryExpr(op, right);
    }

    return primary();
} 

Expr *Parser::primary()
{
    if (match(FALSE))
    {
        return new LiteralExpr(false);
    }
    if (match(TRUE))
    {
        return new LiteralExpr(true);
    }
    if (match(NIL)) 
    {
        return new LiteralExpr(std::monostate());
    }
    if (match(NUMBER, STRING))
    {
        return new LiteralExpr(previous().literal);
    }
    if (match(LEFT_PAREN))
    {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect \")\" after expression");
        return new GroupingExpr(expr);
    }

    throw error(peek(), "Expected expression.");
}

Expr* Parser::parse()
{
    try 
    {
        return expression();
    } catch(ParseError error)
    {
        return nullptr;
    }
}