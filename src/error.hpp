#pragma once
#include <iostream>

inline void report(int line, std::string where, std::string message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

inline void error(int line, std::string message)
{
    report(line, "", message);
}