#pragma once
#include <iostream>
#include <variant>
#include "./token/token_type.hpp"

// std::monostate is equivalent to null
using LiteralValue = std::variant<std::monostate, std::string, double, bool>;

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