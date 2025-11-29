#include "View.h"

void View::showDocumentCreated(int id) const {
            std::cout << "Document was created. Doc ID: " << id << std::endl;
        }

void View::showDocumentImported(const std::string& filename) const {
    std::cout << "Document was imported from " << filename << std::endl;
        }
