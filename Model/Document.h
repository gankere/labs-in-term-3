#pragma once
#include <memory>
#include <vector>
#include "Shape.h"

class Document {
public:
    explicit Document(int id); //запрет на автоматическое создание объекта
    void addPrimitive(std::unique_ptr<Shape> shape);
    int getId() const { return docID; }
private:
    int docID;
    std::vector<std::unique_ptr<Shape>> primitives;
};