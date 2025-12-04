#pragma once
#include <string>

class Shape
{
    private:

    public:
    virtual ~Shape() = default;
    virtual std::string getType() const = 0;
    int getId() const { return theID; }
    int theID;
};
