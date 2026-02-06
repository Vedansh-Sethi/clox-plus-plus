#pragma once
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::pair<std::string, std::string>> splitFields(std::string fields)
{
    // split the arguments in fields
    const std::string del1 = ",";
    std::vector<std::string> splitFields;
    auto pos = fields.find(del1);
    while (pos != std::string::npos)
    {
        splitFields.push_back(fields.substr(0, pos));
        fields.erase(0, pos + del1.length());
        pos = fields.find(del1);
    }
    splitFields.push_back(fields.substr(0, pos));
    fields.erase(0, pos + del1.length());

    // split every field in a pair of {type, fieldName}
    std::vector<std::pair<std::string, std::string>> splitInFields;
    for (std::string field : splitFields)
    {
        std::stringstream pairStream(field);
        std::string type, name;
        pairStream >> type;
        pairStream >> name;
        splitInFields.push_back({type, name});
    }

    return splitInFields;
}