#include <iostream>
#include <sysexits.h>
#include <sstream>
#include <fstream>
#include <string>
#include "scanner/scanner.hpp"
#include "interpreter/interpreter.hpp"
#include "token/token.hpp"
#include "error.hpp"
#include "parser/parser.hpp"
#include "resolver/resolver.hpp"
void run(std::string source)
{
    std::cout << "Starting compilation.." << std::endl;

    std::cout << "Starting Tokenizing..." << std::endl;
    Scanner *scanner = new Scanner(source);
    std::vector<Token> tokens = scanner->scanTokens();

    std::cout << "Starting Parsing..." << std::endl;
    Parser *parser = new Parser(tokens);
    const std::vector<std::unique_ptr<Stmt>>& stmts = parser->parse();


    if (ErrorHandler::hadError)
        return;

    // ASTPrinter *printer = ASTPrinter::getInstance();
    // std::cout << printer->print(expressions) << std::endl;
    Interpreter *interpreter = new Interpreter();

    std::cout << "Starting Resolving..." << std::endl;
    Resolver resolver = Resolver(interpreter);
    resolver.resolve(stmts);

    if (ErrorHandler::hadError)
        return;

    std::cout << "Starting Evaluating..." << std::endl;
    interpreter->interpret(stmts);

    if (ErrorHandler::hadRuntimeError)
        return;
}

void runFile(std::string path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Could not open file:" << path << std::endl;
        exit(EX_IOERR);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    run(source);
    if (ErrorHandler::hadError)
        exit(EX_DATAERR);
    if (ErrorHandler::hadRuntimeError)
        exit(70);
}

void runPrompt()
{
    while (1)
    {
        std::cout << "> ";
        std::string line;
        getline(std::cin, line);
        if (line.empty())
            break;
        run(line);
        ErrorHandler::hadError = false;
        ErrorHandler::hadRuntimeError = false;
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: clox++ [script]" << std::endl;
        exit(EX_USAGE);
    }

    else if (argc == 2)
    {
        runFile(argv[1]);
    }

    else
    {
        runPrompt();
    }
}