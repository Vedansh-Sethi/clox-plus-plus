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
    Expr *expr = ternary();

    if (!check(COMMA))
    {
        return expr;
    }

    std::vector<Expr *> exprs;
    exprs.push_back(expr);

    while (match<TokenType>(COMMA))
    {
        expr = ternary();
        exprs.push_back(expr);
    }

    return new MultiExpr(exprs);
}

Expr *Parser::assignment()
{
    Expr *expr = equality();

    if (match(EQUAL))
    {
        Token equals = previous();
        Expr *value = assignment();
        if(VariableExpr* varExpr = dynamic_cast<VariableExpr*>(expr))
        {
            Token ident = varExpr->ident;
            return new AssignExpr(ident, value);
        }

        ErrorHandler::error(equals, "Invalid Assignment Target");
    }

    return expr;
}

Expr *Parser::ternary()
{
    Expr *expr = assignment();

    Expr *condition = expr;

    if (match<TokenType>(INTERROGATE))
        expr = ternary();
    else
        return expr;

    Expr *ifTrue = expr;

    if (match<TokenType>(COLON))
    {
        expr = ternary();
        Expr *ifFalse = expr;
        expr = new TernaryExpr(condition, ifTrue, ifFalse);
    }
    else
        expr = new TernaryExpr(condition, ifTrue);

    return expr;
}

Expr *Parser::equality()
{
    Expr *expr = comparison();

    while (match<TokenType, TokenType>(BANG_EQUAL, EQUAL_EQUAL))
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

    while (match<TokenType, TokenType>(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
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

    while (match<TokenType, TokenType>(PLUS, MINUS))
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

    while (match<TokenType, TokenType>(SLASH, STAR))
    {
        Token op = previous();
        Expr *right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::unary()
{
    if (match<TokenType, TokenType>(BANG, MINUS))
    {
        Token op = previous();
        Expr *right = unary();
        return new UnaryExpr(op, right);
    }

    return primary();
}

Expr *Parser::primary()
{
    if (match<TokenType>(FALSE))
    {
        return new LiteralExpr(false);
    }
    if (match<TokenType>(TRUE))
    {
        return new LiteralExpr(true);
    }
    if (match<TokenType>(NIL))
    {
        return new LiteralExpr(std::monostate());
    }
    if (match<TokenType>(IDENTIFIER))
    {
        return new VariableExpr(previous());
    }
    if (match<TokenType>(NUMBER, STRING))
    {
        return new LiteralExpr(previous().literal);
    }
    if (match<TokenType>(LEFT_PAREN))
    {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect \")\" after expression");
        return new GroupingExpr(expr);
    }

    ErrorHandler::error(peek(), "Expected Expression");
    advance();
    if (peek().type == END_FILE)
    {
        return nullptr;
    }
    else
    {
        return expression();
    }
}

Stmt *Parser::printStmt()
{
    Expr *value = expression();
    consume(SEMICOLON, "Expected \";\" at the end of line");
    return new PrintStmt(value);
}

Stmt *Parser::exprStmt()
{
    Expr *value = expression();
    consume(SEMICOLON, "Expected \";\" after expression");
    return new ExprStmt(value);
}

Stmt *Parser::statement()
{
    if (match(PRINT))
        return printStmt();

    return exprStmt();
}

Stmt *Parser::varDeclStmt()
{
    Token ident = consume(IDENTIFIER, "Expected a variable name");

    Expr *initializer = nullptr;
    if (match(EQUAL))
    {
        initializer = expression();
    }

    consume(SEMICOLON, "Expected \";\" after variable declaration");
    return new VarDeclStmt(ident, initializer);
}

Stmt *Parser::declaration()
{
    try
    {
        if (match(VAR))
        {
            return varDeclStmt();
        }

        return statement();
    }
    catch (ParseError error)
    {
        synchronize();
        return nullptr;
    }
}

std::vector<Stmt *> Parser::parse()
{
    std::vector<Stmt *> statements;
    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}