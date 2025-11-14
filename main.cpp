#include <iostream>
#include <map>
#include "my_container.h"
#include "my_allocator.h"

long long factorial(int n) {
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    std::map<int, int> map1;
    
    for (int i = 0; i < 10; ++i) {
        map1[i] = factorial(i);
    }

    // 3) создание std::map с вашим аллокатором
    std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>>> map2;

    // 4) заполнение 10 элементами
    for (int i = 0; i < 10; ++i) {
        map2[i] = factorial(i);
    }

    // 5) вывод значений
    for (const auto& [key, value] : map2) {
        std::cout << key << " " << value << "\n";
    }

    // ✅ Вывести сколько байт выделено в map2
    std::cout << "Allocated bytes in map2: " << map2.get_allocator().allocated() << "\n";

    // 6) создание своего контейнера
    my_container<int> mycontainer1;

    // 7) заполнение
    for (int i = 0; i < 10; ++i) {
        mycontainer1.push_back(i);
    }

    // 8) создание своего контейнера с аллокатором
    my_container<int, my_allocator<int>> mycontainer2;

    // 9) заполнение
    for (int i = 0; i < 10; ++i) {
        mycontainer2.push_back(i);
    }

    // 10) вывод
    for (auto it = mycontainer2.begin(); it != mycontainer2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // ✅ Вывести сколько байт выделено в mycontainer2
    std::cout << "Allocated bytes in mycontainer2: " << mycontainer2.get_allocator().allocated() << "\n";

    return 0;
}