#include <iostream>
#include <sysexits.h>
#include <vector>
#include <string>
#include "utils/generate_ast.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: generate_expr <output directory>" << std::endl;
        exit(EX_IOERR);
    }
    std::string outputDir = argv[1];
    std::vector<std::string> types =
        {
            "Binary   : Expr* left, Token op, Expr* right",
            "Grouping : Expr* expression",
            "Literal  : LiteralValue value",
            "Unary    : Token op, Expr* right",
            "Multi : std::vector<Expr*> exprs",
            "Ternary : Expr* condition, Expr* ifTrue, Expr* ifFalse",
        };
    defineAST(outputDir, "expr", "Expr", types);
}