#include "View.h"

void View::showDocumentCreated(int id) const {
    std::cout << "Document was created. Doc ID: " << id << std::endl;
        }

void View::showDocumentImported(const std::string& filename) const {
    std::cout << "Document was imported from " << filename << std::endl;
        }

void View::showDocumentExported(const std::string& filename) const {
    std::cout << "Document was exported " << filename << std::endl;
        }
void View::showPrimitiveAdded(int docId, const std::string& type, int shapeId) const {
        std::cout << "В документ №" << docId
                  << " добавлен примитив: " << type
                  << " №" << shapeId << std::endl;
    }