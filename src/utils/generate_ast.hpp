#include <iostream>
#include <sysexits.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "utils/splitter.hpp"
#include <error.hpp>

void defineType(std::stringstream& writer, std::string baseName, std::string className, std::string fields)
{
    writer << "class " << className << " : public " << baseName << " {" << std::endl << std::endl;
    std::vector<std::pair<std::string, std::string>> fieldsVector = splitFields(fields);

    // writing member variables
    for(std::pair<std::string, std::string> fieldPair : fieldsVector)
    {
        writer << "     const " << fieldPair.first << " " << fieldPair.second << ";" << std::endl;
    }
    writer << std::endl;

    // writing the constructor for the expression class
    writer << "     " << className << "(" << fields << ") : ";
    for(size_t i = 0; i < fieldsVector.size(); i++)
    {
        writer << fieldsVector[i].second << "(" << fieldsVector[i].second << ")";
        if(i != fieldsVector.size() - 1) writer << ", "; 
    }

    writer << " {}" << std::endl;

    writer << std::endl << " };" << std::endl << std::endl;
}

void defineAST(std::string outputDir, std::string fileName, std::string baseName, std::vector<std::string> types)
{
    std::string path = outputDir + "/" + fileName + ".hpp";

    std::ofstream outFile(path);
    if(!outFile.is_open())
    {
        std::cerr << "Could not open output file at path:" << path << std::endl;
        exit(EX_IOERR);
    }

    std::stringstream writer;
    writer << "#include <iostream>" << std::endl;
    writer << "#include \"token/token.hpp\"" << std::endl << std::endl;
    writer << "class " << baseName << " {};" << std::endl << std::endl;
    for(std::string type : types)
    {
        std::string arg;
        std::string className;
        std::string fields;
        std::stringstream typeStream(type);
        typeStream >> arg;
        className = arg;
        typeStream >> arg;
        while(typeStream >> arg)
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
