#include "Controller/Controller.h"

int main() {
    Controller ctrl;
    ctrl.createNewDocument();
    ctrl.importDocument("data.txt");
    return 0;
}