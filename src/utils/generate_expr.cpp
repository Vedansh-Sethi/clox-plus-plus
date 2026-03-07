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
            "Assign : Token ident, std::unique_ptr<Expr> value",
            "Binary   : std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right",
            "Grouping : std::unique_ptr<Expr> expression",
            "Literal  : LiteralValue value",
            "Unary    : Token op, std::unique_ptr<Expr> right",
            "Multi : std::vector<std::unique_ptr<Expr>> exprs",
            "Ternary : std::unique_ptr<Expr> condition, std::unique_ptr<Expr> ifTrue, std::unique_ptr<Expr> ifFalse",
            "Variable : Token ident",
            "Logical : std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right",
        };
    defineAST(outputDir, "expr", "Expr", types);
}