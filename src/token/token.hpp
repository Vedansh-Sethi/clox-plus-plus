#pragma once
#include <iostream>
#include <variant>
#include <memory>
#include "token/token_type.hpp"

class Callable;
class Instance;

// std::monostate is equivalent to null
using LiteralValue = std::variant<std::monostate, std::string, double, bool, Callable*, Instance*>;

struct LiteralPrinter
{
    std::string operator()(std::monostate) const { return "nil"; }
    std::string operator()(const std::string &s) const { return s; }
    std::string operator()(double d) const { return std::to_string(d); }
    std::string operator()(bool b) const { return b ? "true" : "false"; }
    std::string operator()(Callable* c) const;
    std::string operator()(Instance* i) const;
};

std::string literalToString(const LiteralValue& val);

class Token
{
public:
    const TokenType type;
    const std::string lexeme;
    const int line;
    const LiteralValue literal;

    Token(TokenType type, std::string lexeme, LiteralValue literal, int line) : type(type),
                                                                                lexeme(lexeme),
                                                                                line(line),
                                                                                literal(literal)
    {
    }

    std::string toString() const;

private:
    std::string literalToString() const;
};