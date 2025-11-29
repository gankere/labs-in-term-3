#include "Controller.h"

void Controller::createNewDocument() {
    currentDocument = std::make_unique<Document>(nextDocId);
    view.showDocumentCreated(nextDocId);
    ++nextDocId;
}

void Controller::importDocument(const std::string& filename){
    view.showDocumentImported(filename);
}