#pragma once
#include <iostream>

class View {
    public:
        void showDocumentCreated(int id) const;
        void showDocumentImported(const std::string& filename) const;
        void showDocumentExported(const std::string& filename) const;
    private:
};