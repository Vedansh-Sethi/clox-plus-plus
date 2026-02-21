#include <string>
#include <vector>
#include <variant>
#include <map>
#include "scanner/scanner.hpp"
#include "error.hpp"

static std::map<std::string, TokenType> keywords =
    {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}};

inline bool Scanner::isAtEnd() const
{
    return current >= (int)source.length();
}

inline char Scanner::advance()
{
    return source.at(current++);
}

inline void Scanner::addToken(TokenType type, LiteralValue literal)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

inline void Scanner::addToken(TokenType type)
{
    addToken(type, "");
}

inline bool Scanner::match(char expected)
{
    if (isAtEnd())
        return false;
    if (source.at(current) != expected)
        return false;

    current++;
    return true;
}

inline char Scanner::peek(int steps) const
{
    if (current + steps >= (int)source.length())
        return '\0';
    return source.at(current + steps);
}

// handling string literals
void Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
    {
        ErrorHandler::error(line, "Unterminated string");
        return;
    }

    // consumes the left over '"'
    advance();
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(STRING, value);
}

// handles number literals
void Scanner::number()
{
    while (isdigit(peek()))
        advance();
    if (peek() == '.' && isdigit(peek(1)))
    {
        advance();
        while (isdigit(peek()))
            advance();
    }
    std::string stringValue = source.substr(start, current - start);
    double doubleValue = std::stod(stringValue);
    addToken(NUMBER, doubleValue);
}

// handles reserved keywords and identifiers
void Scanner::identifier()
{
    while (isalnum(peek()))
        advance();
    TokenType type = IDENTIFIER;
    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    if (it != keywords.end())
        type = it->second;

    addToken(type);
}

// handles multiline comments
void Scanner::multilineComment()
{
    while (!isAtEnd())
    {
        if (peek() == '/' && peek(1) == '*')
        {
            advance();
            advance();
            multilineComment();
            continue;
        }
        if (peek() == '*' && peek(1) == '/')
        {
            advance();
            advance();
            return;
        }
        if (peek() == '\n')
        {
            line++;
        }
        advance();
    }
    if (isAtEnd())
    {
        ErrorHandler::error(line, "Unterminated Multiline Comment");
    }
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c)
    {
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case ';':
        addToken(SEMICOLON);
        break;
    case '*':
        addToken(STAR);
        break;
    case '?':
        addToken(INTERROGATE);
        break;
    case ':':
        addToken(COLON);
        break;
    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (match('/'))
        {
            // while the character ahead is not '\n', and we are not at the end of source file, we advance
            while (peek() != '\n' && !isAtEnd())
                advance();
        }
        else if (match('*'))
        {
            multilineComment();
            break;
        }
        else
        {
            addToken(SLASH);
        }
        break;
    case '\n':
        line++;
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '"':
        string();
        break;
    default:
        if (isdigit(c))
        {
            number();
        }
        else if (isalpha(c))
        {
            identifier();
        }
        else
        {
            ErrorHandler::error(line, "Unexpected Character");
        }
        break;
    }
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(END_FILE, "", std::monostate(), line));
    return tokens;
}

void Scanner::printTokens(std::vector<Token> tokens) const
{
    for (Token token : tokens)
    {
        std::cout << token.toString() << std::endl;
    }
}