#include <iostream>
#include <sysexits.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "utils/splitter.hpp"
#include <error.hpp>

void defineType(std::stringstream &writer, std::string baseName, std::string className, std::string fields)
{
    writer << "class " << className << baseName << " : public " << baseName << " {" << std::endl
           << std::endl;
    std::vector<std::pair<std::string, std::string>> fieldsVector = splitFields(fields);

    // writing member variables
    writer << "public: " << std::endl;
    for (std::pair<std::string, std::string> fieldPair : fieldsVector)
    {
        writer << "     " << fieldPair.first << " " << fieldPair.second << ";" << std::endl;
    }
    writer << std::endl;

    // writing the constructor for the expression class
    writer << "     " << className << baseName << "(" << fields << ") : ";
    for (size_t i = 0; i < fieldsVector.size(); i++)
    {
        writer << fieldsVector[i].second << "(" << fieldsVector[i].second << ")";
        if (i != fieldsVector.size() - 1)
            writer << ", ";
    }
    writer << " {}" << std::endl << std::endl;

    // writing the visitor function for each subclass
    writer << "     void accept(" << baseName << "Visitor* visitor) override {" << std::endl;
    writer << "         visitor->visit" << className << baseName << "(this);" << std::endl;
    writer << "     }";
    writer << std::endl
           << " };" << std::endl
           << std::endl;
}

void defineAST(std::string outputDir, std::string fileName, std::string baseName, std::vector<std::string> types)
{
    std::string path = outputDir + "/" + fileName + ".hpp";

    std::ofstream outFile(path);
    if (!outFile.is_open())
    {
        std::cerr << "Could not open output file at path:" << path << std::endl;
        exit(EX_IOERR);
    }
    std::stringstream writer;

    // file imports
    writer << "#include <iostream>" << std::endl;
    writer << "#include \"token/token.hpp\"" << std::endl
           << std::endl;

    // main body
    writer << "class " << baseName <<"Visitor;" << std::endl;
    for (std::string type : types)
    {
        std::string arg;
        std::string className;
        std::string fields;
        std::stringstream typeStream(type);
        typeStream >> arg;
        className = arg;
        typeStream >> arg;
        while (typeStream >> arg)
        {
            fields += arg;
            typeStream >> arg;
            fields += " " + arg;
        }
        writer << "class " << className << baseName << ";" << std::endl;
    }
    writer << std::endl
           << std::endl;

    // base class definition
    writer << "class " << baseName << " {" << std::endl;
    writer << "public: " << std::endl;
    writer << "     virtual void accept(" << baseName << "Visitor* visitor) = 0;" << std::endl;
    writer << "};" << std::endl
           << std::endl;

    // Visitor class definition
    writer << "class " << baseName << "Visitor {" << std::endl
           << std::endl;
    writer << "public:" << std::endl;
    for (std::string type : types)
    {
        std::string arg;
        std::string className;
        std::string fields;
        std::stringstream typeStream(type);
        typeStream >> arg;
        className = arg;
        typeStream >> arg;
        while (typeStream >> arg)
        {
            fields += arg;
            typeStream >> arg;
            fields += " " + arg;
        }
        writer << "     virtual void visit" << className << baseName << "(" << className << baseName << "* " << baseName << ") = 0;" << std::endl;
    }
    writer << std::endl;
    writer << "};" << std::endl << std::endl;

    for (std::string type : types)
    {
        std::string arg;
        std::string className;
        std::string fields;
        std::stringstream typeStream(type);
        typeStream >> arg;
        className = arg;
        typeStream >> arg;
        while (typeStream >> arg)
        {
            fields += arg;
            typeStream >> arg;
            fields += " " + arg;
        }
        defineType(writer, baseName, className, fields);
    }

    outFile << writer.str();
    outFile.close();
}
