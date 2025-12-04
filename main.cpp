#include "Controller/Controller.h"

int main() {
    Controller ctrl;
    ctrl.createNewDocument();
    ctrl.importDocument("importedData.txt");
    ctrl.exportDocument("exportedData.txt");
    return 0;
}