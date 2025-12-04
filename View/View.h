#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "../Model/Shape.h"

class View {
    public:
        void showDocumentCreated(int id) const;
        void showDocumentImported(const std::string& filename) const;
        void showDocumentExported(const std::string& filename) const;
        void showPrimitiveAdded(int docId, const std::string& type, int shapeId) const;
        void showPrimitiveRemoved(int docId, int shapeId) const;
    private:
};