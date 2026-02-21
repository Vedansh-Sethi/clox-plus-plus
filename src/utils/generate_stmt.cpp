#include <iostream>
#include "utils/generate_ast.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./generate_stmt <Path to directory>" << std::endl;
        exit(EX_IOERR);
    }
    std::string outputDir = argv[1];
    std::vector<std::string> types =
        {
            "Expr : Expr* expr",
            "Print : Expr* expr",
            "VarDecl : Token ident, Expr* init",
        };
    defineAST(outputDir, "stmt", "Stmt", types);
}