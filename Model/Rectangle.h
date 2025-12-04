#pragma once
#include "Shape.h"
#include <string>

class Rectangle : public Shape 
{
    public:
    explicit Rectangle(int id) { theID = id; }
    std::string getType() const override { return "Rectangle"; }
};
