#include "Controller.h"
#include "../Model/Circle.h"
#include "../Model/Rectangle.h"

void Controller::createNewDocument() {
    currentDocument = std::make_unique<Document>(nextDocId);
    view.showDocumentCreated(nextDocId);
    ++nextDocId;
}

void Controller::importDocument(const std::string& filename){
    view.showDocumentImported(filename);
}

void Controller::exportDocument (const std::string& filename){
    view.showDocumentExported(filename);
}

int Controller::nextShapeId = 1;

void Controller::createPrimitive(const std::string& type) {
    if (!currentDocument) { //дока нет? - автосоздание
        currentDocument = std::make_unique<Document>(nextDocId++);
    }

    std::unique_ptr<Shape> shape;
    if (type == "Circle") {
        shape = std::make_unique<Circle>(nextShapeId);
    }
    if (type == "Rectangle") { 
    shape = std::make_unique<Rectangle>(nextShapeId);
    }

    if (shape) {
        int shapeId = shape->getId();          
        std::string shapeType = shape->getType(); 
        int docId = currentDocument->getId();

        currentDocument->addPrimitive(std::move(shape)); //передача указателя
        view.showPrimitiveAdded(docId, shapeType, shapeId);

        ++nextShapeId;
}
}