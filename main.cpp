#include "lib.h"

#include <iostream>

int main (int, char **) {
    std::cout << "Hello, world!" << std::endl;
    std::cout << "Version: 1.0." << version() << std::endl;
    return 0;
}
