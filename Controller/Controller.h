#pragma once
#include <memory>
#include <string>
#include "../Model/Document.h"
#include "../View/View.h"

class Controller {
public:
    void createNewDocument();
    void importDocument(const std::string& filename);

private:
    std::unique_ptr<Document> currentDocument;
    View view;
    int nextDocId = 1;
};