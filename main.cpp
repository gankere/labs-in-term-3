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
    std::cout << "1.) Создание экземпляра std::map\n";
    std::map<int, int> map1;
    
    std::cout << "2.) Заполнение 10 элементами (ключ - число от 0 до 9, значение - факториал ключа)\n";
    for (int i = 0; i < 10; ++i) {
        map1[i] = factorial(i);
    }

    std::cout << "3.) Создание экземпляра std::map<int, int> с новым аллокатором\n";
    // 4) заполнение 10 элементами
    std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>>> map2;

    std::cout << "4.) Заполнение 10 элементами (ключ - число от 0 до 9, значение - факториал ключа)\n";
    for (int i = 0; i < 10; ++i) {
        map2[i] = factorial(i);
    }

    std::cout << "5.) Вывод на экран всех значений (ключ и значение разделены пробелом)\n";
    for (const auto& [key, value] : map2) {
        std::cout << key << " " << value << "\n";
    }

    std::cout << "Allocated bytes in map2: " << map2.get_allocator().allocated() << "\n";

    std::cout << "6.) Создание экземпляра своего контейнера для хранения значений типа int\n";
    my_container<int> mycontainer1;

    std::cout << "7.) Заполнение 10 элементами от 0 до 9\n";
    for (int i = 0; i < 10; ++i) {
        mycontainer1.push_back(i);
    }

    std::cout << "8.) Создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором\n";
    my_container<int, my_allocator<int>> mycontainer2;

    std::cout << "9.) Заполнение 10 элементами от 0 до 9\n";
    for (int i = 0; i < 10; ++i) {
        mycontainer2.push_back(i);
    }

    std::cout << "10.) Вывод на экран всех значений, хранящихся в контейнере\n";
    for (auto it = mycontainer2.begin(); it != mycontainer2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Allocated bytes in mycontainer2: " << mycontainer2.get_allocator().allocated() << "\n";

    return 0;
}