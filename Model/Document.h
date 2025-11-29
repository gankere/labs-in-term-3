#pragma once

class Document {
public:
    explicit Document(int id); //запрет на автоматическое создание объекта
private:
    int docID;
};