#include "Document.h"

Document::Document(int id) : docID(id) {}

void Document::addPrimitive(std::unique_ptr<Shape> shape) {
    if (shape) {
        primitives .push_back(std::move(shape));
    }
}
void Document::removePrimitive(int shapeId) {
    // В реальности: найти и удалить из vector
    // В макете: просто подготовка к вызову — лог будет в View через Controller
}