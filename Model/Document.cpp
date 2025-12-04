#include "Document.h"

Document::Document(int id) : docID(id) {}

void Document::addPrimitive(std::unique_ptr<Shape> shape) {
    if (shape) {
        primitives .push_back(std::move(shape));
    }
}