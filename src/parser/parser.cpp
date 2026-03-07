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

std::unique_ptr<Expr> Parser::expression()
{
    return commaSeparatedExpressions();
}

std::unique_ptr<Expr> Parser::commaSeparatedExpressions()
{
    std::unique_ptr<Expr> expr = ternary();

    if (!check(COMMA))
    {
        return expr;
    }

    std::vector<std::unique_ptr<Expr>> exprs;
    exprs.push_back(expr);

    while (match<TokenType>(COMMA))
    {
        expr = ternary();
        exprs.push_back(expr);
    }

    return std::make_unique<MultiExpr>(exprs);
}

std::unique_ptr<Expr> Parser::assignment()
{
    std::unique_ptr<Expr> expr = logical_or();

    if (match(EQUAL))
    {
        Token equals = previous();
        std::unique_ptr<Expr> value = assignment();
        if (VariableExpr *varExpr = dynamic_cast<VariableExpr *>(expr.get()))
        {
            Token ident = varExpr->ident;
            return std::make_unique<AssignExpr>(ident, std::move(value));
        }

        ErrorHandler::error(equals, "Invalid Assignment Target");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logical_or()
{
    std::unique_ptr<Expr> expr = logical_and();

    while (!isAtEnd() && match(OR))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = logical_or();
        expr = std::make_unique<LogicalExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logical_and()
{
    std::unique_ptr<Expr> expr = equality();

    while (match(AND))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<LogicalExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::ternary()
{
    std::unique_ptr<Expr> expr = assignment();

    std::unique_ptr<Expr> condition = std::move(expr);

    if (match<TokenType>(INTERROGATE))
        expr = ternary();
    else
        return expr;

    std::unique_ptr<Expr> ifTrue = std::move(expr);

    if (match<TokenType>(COLON))
    {
        expr = ternary();
        std::unique_ptr<Expr> ifFalse = std::move(expr);
        expr = std::make_unique<TernaryExpr>(condition, ifTrue, ifFalse);
    }
    else
        expr = std::make_unique<TernaryExpr>(condition, ifTrue, nullptr);

    return expr;
}

std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> expr = comparison();

    while (match<TokenType, TokenType>(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> expr = term();

    while (match<TokenType, TokenType>(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term()
{
    std::unique_ptr<Expr> expr = factor();

    while (match<TokenType, TokenType>(PLUS, MINUS))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor()
{
    std::unique_ptr<Expr> expr = unary();

    while (match<TokenType, TokenType>(SLASH, STAR))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<BinaryExpr>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match<TokenType, TokenType>(BANG, MINUS))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<BinaryExpr>(op, right);
    }

    return call();
}

std::unique_ptr<Expr> Parser::finishCall(const std::unique_ptr<Expr>& callee)
{
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!match(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
            {
                error(peek(), "Function can't have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN, "Expected \")\" after arguments");

    return std::make_unique<CallExpr>(callee, paren, arguments);
}

std::unique_ptr<Expr> Parser::call()
{
    std::unique_ptr<Expr> expr = primary();

    while (true)
    {
        if (match(LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else
            break;
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match<TokenType>(FALSE))
    {
        return std::make_unique<LiteralExpr>(false);
    }
    if (match<TokenType>(TRUE))
    {
        return std::make_unique<LiteralExpr>(true);
    }
    if (match<TokenType>(NIL))
    {
        return std::make_unique<LiteralExpr>(std::monostate());
    }
    if (match<TokenType>(IDENTIFIER))
    {
        return std::make_unique<VariableExpr>(previous());
    }
    if (match<TokenType>(NUMBER, STRING))
    {
        return std::make_unique<LiteralExpr>(previous().literal);
    }
    if (match<TokenType>(LEFT_PAREN))
    {
        std::unique_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect \")\" after expression");
        return std::make_unique<GroupingExpr>(expr);
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

std::unique_ptr<Stmt> Parser::printStmt()
{
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected \";\" at the end of line");
    return std::make_unique<PrintStmt>(value);
}

std::unique_ptr<Stmt> Parser::exprStmt()
{
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expected \";\" after expression");
    return std::make_unique<ExprStmt>(value);
}

std::unique_ptr<Stmt> Parser::ifStmt()
{
    consume(LEFT_PAREN, "Expected \"(\" at the start of if statement");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected \")\" after end of expression");
    std::unique_ptr<Stmt> trueStmt = statement();
    std::unique_ptr<Stmt> falseStmt = nullptr;
    if (match<TokenType>(ELSE))
    {
        falseStmt = statement();
    }
    return std::make_unique<IfStmt>(condition, trueStmt, falseStmt);
}

std::unique_ptr<Stmt> Parser::continueStmt()
{
    Token keyword = previous();
    consume(SEMICOLON, "Expected \";\" after keyword");
    if (loopCount == 0)
    {
        throw error(peek(), "Expected continue statement inside a loop");
    }

    return std::make_unique<ContinueStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::breakStmt()
{
    Token keyword = previous();
    consume(SEMICOLON, "Expected \";\" after statement");
    if (loopCount == 0)
    {
        throw error(peek(), "Expected break statement inside a loop");
    }

    return std::make_unique<BreakStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::whileStmt()
{
    loopCount++;
    consume(LEFT_PAREN, "Expected \"(\" at the start of while");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected \")\" after expression");
    std::unique_ptr<Stmt> task = statement();
    loopCount--;
    return std::make_unique<WhileStmt>(condition, task);
}

std::unique_ptr<Stmt> Parser::forStmt()
{
    loopCount++;
    consume(LEFT_PAREN, "Expected \"(\" at the start of for");
    std::unique_ptr<Stmt> initializer;
    if (match(SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(VAR))
    {
        initializer = varDeclStmt();
    }
    else
    {
        initializer = exprStmt();
    }

    std::unique_ptr<Expr> condition = nullptr;
    if (!match(SEMICOLON))
    {
        condition = expression();
    }
    consume(SEMICOLON, "Expected \";\" after expression");

    std::unique_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expected \")\" after expressions");

    std::unique_ptr<Stmt> task = statement();

    loopCount--;
    return std::make_unique<ForStmt>(initializer, condition, increment, task);
}

std::unique_ptr<Stmt> Parser::statement()
{
    if (match(PRINT))
        return printStmt();
    if (match<TokenType>(LEFT_BRACE))
        return blockStmt();
    if (match<TokenType>(IF))
        return ifStmt();
    if (match<TokenType>(WHILE))
        return whileStmt();
    if (match<TokenType>(FOR))
        return forStmt();
    if (match<TokenType>(BREAK))
        return breakStmt();
    if (match<TokenType>(CONTINUE))
        return continueStmt();
    return exprStmt();
}

std::unique_ptr<Stmt> Parser::varDeclStmt()
{
    Token ident = consume(IDENTIFIER, "Expected a variable name");

    std::unique_ptr<Expr> initializer = nullptr;
    if (match(EQUAL))
    {
        initializer = expression();
    }

    consume(SEMICOLON, "Expected \";\" after variable declaration");
    return std::make_unique<VarDeclStmt>(ident, initializer);
}

std::unique_ptr<Stmt> Parser::declaration()
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

std::unique_ptr<Stmt> Parser::blockStmt()
{
    std::vector<std::unique_ptr<Stmt> > stmts;

    while (!check(RIGHT_BRACE) && !isAtEnd())
    {
        stmts.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expected \"}\" after block");

    return std::make_unique<BlockStmt>(stmts);
}

std::vector<std::unique_ptr<Stmt> > Parser::parse()
{
    std::vector<std::unique_ptr<Stmt> > statements;
    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}