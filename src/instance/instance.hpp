#pragma once
#include <unordered_map>
#include "class/class.hpp"

using Fields = std::unordered_map<std::string, LiteralValue>;

class Instance : public std::enable_shared_from_this<Instance>
{
    std::shared_ptr<Class> klass;
    Fields fields;

public:
    Instance(std::shared_ptr<Class> klass) : klass(klass) {}
    LiteralValue get(Token name);
    void set(Token name, LiteralValue value);
    std::string toString() const;
};