#pragma once
#include <iostream>

class View {
    public:
        void showDocumentCreated(int id) const {
            std::cout << "Document was created. Doc ID: " << id << std::endl;
        }
        
    private:
};