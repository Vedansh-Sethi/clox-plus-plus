#include <iostream>
#include <sysexits.h>
#include <sstream>
#include <fstream>
#include <string>
#include "./scanner/scanner.hpp"
#include "./token/token.hpp"
#include "error.hpp"

bool hadError = false;

void run(std::string source)
{
    Scanner scanner = Scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for (Token token : tokens)
    {
        std::cout << token.toString() << std::endl;
    }
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
    if (hadError) exit(EX_DATAERR);
}

void runPrompt()
{
    while (1)
    {
        std::cout << "> " << std::endl;
        std::string line;
        getline(std::cin, line);
        if (line.empty())
            break;
        run(line);
        hadError = true;
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