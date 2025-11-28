#pragma once
#include <memory>
#include "../Model/Document.h"
#include "../View/View.h"

class Controller {
public:
    void createNewDocument();

private:
    std::unique_ptr<Document> currentDocument;
    View view;
    int nextDocId = 1;
};