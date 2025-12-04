#pragma once
#include "Shape.h"
#include <string>

class Circle : public Shape 
{
    public:
    explicit Circle(int id) { theID = id; }
    std::string getType() const override { return "Circle"; }
};
