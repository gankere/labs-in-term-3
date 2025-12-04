#include "Controller/Controller.h"

int main() {
    Controller ctrl;
    ctrl.createNewDocument();
    ctrl.importDocument("importedData.txt");
    ctrl.exportDocument("exportedData.txt");
    ctrl.createPrimitive("Circle");
    
    ctrl.createNewDocument();
    ctrl.createPrimitive("Rectangle");
    ctrl.createPrimitive("Circle");
    return 0;
}