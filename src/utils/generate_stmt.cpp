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
            "Block : std::vector<std::unique_ptr<Stmt>> stmts",
            "Expr : std::unique_ptr<Expr> expr",
            "Print : std::unique_ptr<Expr> expr",
            "Return : Token keyword, std::unique_ptr<Expr> value",
            "VarDecl : Token ident, std::unique_ptr<Expr> init",
            "If : std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> trueStmt, std::unique_ptr<Stmt> falseStmt",
            "While : std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> task",
            "For : std::unique_ptr<Stmt> initializer, std::unique_ptr<Expr> condition, std::unique_ptr<Expr> increment, std::unique_ptr<Stmt> task",
            "Break : Token keyword",
            "Continue : Token keyword",
            "FunctionDecl : Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body",
            "GetterDecl : Token keyword, std::vector<std::unique_ptr<Stmt>> body",
            "SetterDecl : Token keyword, Token param, std::vector<std::unique_ptr<Stmt>> body",
            "Property : Token name, std::unique_ptr<GetterDeclStmt> getter, std::unique_ptr<SetterDeclStmt> setter",
            "ClassDecl : Token name, std::vector<std::unique_ptr<FunctionDeclStmt>> methods, std::vector<std::unique_ptr<FunctionDeclStmt>> statics, std::vector<std::unique_ptr<PropertyStmt>> properties",
        };
    defineAST(outputDir, "stmt", "Stmt", types);
}