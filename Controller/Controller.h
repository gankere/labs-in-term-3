#pragma once
#include <memory>
#include <string>
#include "../Model/Document.h"
#include "../View/View.h"
#include "../Model/Circle.h" 

class Controller {
public:
    void createNewDocument();
    void importDocument(const std::string& filename);
    void exportDocument(const std::string& filename);
    void createPrimitive(const std::string& type);
private:
    std::unique_ptr<Document> currentDocument;
    View view;
    int nextDocId = 1;
    static int nextShapeId;
};

