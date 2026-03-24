#include <variant>
#include <memory>
#include "token/token.hpp"
#include "callable/callable.hpp"
#include "instance/instance.hpp"
#include <string>
class Callable;

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    // Single-character tokens
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case MODULUS:
        return "MODULUS";
    case COMMA:
        return "COMMA";
    case DOT:
        return "DOT";
    case MINUS:
        return "MINUS";
    case PLUS:
        return "PLUS";
    case COLON:
        return "COLON";
    case INTERROGATE:
        return "INTERROGATE";
    case SEMICOLON:
        return "SEMICOLON";
    case SLASH:
        return "SLASH";
    case STAR:
        return "STAR";

    // One or two character tokens
    case BANG:
        return "BANG";
    case BANG_EQUAL:
        return "BANG_EQUAL";
    case EQUAL:
        return "EQUAL";
    case EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case GREATER:
        return "GREATER";
    case GREATER_EQUAL:
        return "GREATER_EQUAL";
    case LESS:
        return "LESS";
    case LESS_EQUAL:
        return "LESS_EQUAL";

    // Literals
    case IDENTIFIER:
        return "IDENTIFIER";
    case STRING:
        return "STRING";
    case NUMBER:
        return "NUMBER";

    // Keywords
    case AND:
        return "AND";
    case CLASS:
        return "CLASS";
    case ELSE:
        return "ELSE";
    case FALSE:
        return "FALSE";
    case FUN:
        return "FUN";
    case FOR:
        return "FOR";
    case IF:
        return "IF";
    case NIL:
        return "NIL";
    case OR:
        return "OR";
    case PRINT:
        return "PRINT";
    case RETURN:
        return "RETURN";
    case SUPER:
        return "SUPER";
    case THIS:
        return "THIS";
    case TRUE:
        return "TRUE";
    case VAR:
        return "VAR";
    case WHILE:
        return "WHILE";
    case BREAK:
        return "BREAK";
    case CONTINUE:
        return "CONTINUE";
    
    case END_FILE:
        return "EOF";
    default:
        return "";
    }
}


std::string LiteralPrinter::operator()(Callable* c) const
{
    return c->toString();
}

std::string LiteralPrinter::operator()(Instance* i) const
{
    return i->toString();
}

std::string literalToString(const LiteralValue& val)
{
    return std::visit(LiteralPrinter{}, val);
}

std::string Token::literalToString() const
{
    if (std::holds_alternative<std::monostate>(literal))
        return "nil";
    if (std::holds_alternative<std::string>(literal))
        return std::get<std::string>(literal);
    if (std::holds_alternative<double>(literal))
        return std::to_string(std::get<double>(literal));
    if (std::holds_alternative<bool>(literal))
        return std::get<bool>(literal) ? "true" : "false";
    if(std::holds_alternative<Callable*>(literal))
    {
        return std::get<Callable*>(literal)->toString();
    }
    if(std::holds_alternative<Instance*>(literal))
    {
        return std::get<Instance*>(literal)->toString();
    }
    return "";
}

std::string Token::toString() const
{
    return tokenTypeToString(type) + " " + lexeme + " " + literalToString();
}