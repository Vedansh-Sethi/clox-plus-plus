#pragma once
#include <iostream>
#include <variant>
#include "token/token_type.hpp"

// std::monostate is equivalent to null
using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

struct LiteralPrinter
{
    std::string operator()(std::monostate) {return "nil";}
    std::string operator()(std::string s){return s;}
    std::string operator()(double d){return std::to_string(d);}
    std::string operator()(bool b){return b ? "true" : "false";}
};

inline std::string literalToString(LiteralValue val)
{
    return std::visit(LiteralPrinter{}, val);
}

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