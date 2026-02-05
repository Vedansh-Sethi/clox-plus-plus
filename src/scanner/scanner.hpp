#pragma once
#include <string>
#include <vector>
#include "./token/token.hpp"
#include "./token/token_type.hpp"

class Scanner
{
private:
    const std::string source;
    std::vector<Token> tokens;
    inline bool isAtEnd() const;
    void scanToken();
    inline bool match(char expected);
    inline char peek(int steps = 0) const;
    void string();
    void number();
    void identifier();
    void multilineComment();
    // start is the start of current token in the source
    // current is the character at which scanner currently is
    int start = 0;
    int current = 0;
    int line = 1;

public:
    Scanner(std::string source) : source(source) {}

    std::vector<Token> scanTokens();

private:
    inline char advance();
    inline void addToken(TokenType token);
    inline void addToken(TokenType token, LiteralValue literal);
};