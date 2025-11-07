#include <iostream>
#include <map>
#include "my_allocator.h"
#include "my_container.h"

// Вспомогательная функция для вычисления факториала
long long factorial(int n) {
    if (n < 0) return 0;
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    // 1) создание экземпляра std::map<int, int>;
    std::cout << "=== 1. std::map<int, int> ===\n";
    std::map<int, int> map1;

    // 2) заполнение 10 элементами, где ключ – это число от 0 до 9, а
    // значение – факториал ключа;
    for (int i = 0; i < 10; ++i) {
        map1[i] = static_cast<int>(factorial(i));
    }

    // 5) вывод на экран всех значений (ключ и значение разделены пробелом)
    std::cout << "=== 2. Вывод std::map<int, int> ===\n";
    for (const auto& pair : map1) {
        std::cout << pair.first << " " << pair.second << "\n";
    }

    // 3) создание экземпляра std::map<int, int> с новым аллокатором,
    // ограниченным 10 элементами;
    std::cout << "\n=== 3. std::map<int, int> с my_allocator<int, 10> ===\n";
    std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>, 10>> map2;

    // 4) заполнение 10 элементами, где ключ – это число от 0 до 9, а значение
    // – факториал ключа;
    for (int i = 0; i < 10; ++i) {
        map2[i] = static_cast<int>(factorial(i));
    }

    // 5) вывод на экран всех значений (ключ и значение разделены пробелом)
    std::cout << "=== 4. Вывод std::map<int, int> с my_allocator ===\n";
    for (const auto& pair : map2) {
        std::cout << pair.first << " " << pair.second << "\n";
    }

    // 6) создание экземпляра своего контейнера для хранения значений типа int;
    std::cout << "\n=== 5. my_container<int> с std::allocator ===\n";
    my_container<int> container1;

    // 7) заполнение 10 элементами от 0 до 9;
    for (int i = 0; i < 10; ++i) {
        container1.push_back(i);
    }

    // 10) вывод на экран всех значений, хранящихся в контейнере.
    std::cout << "=== 6. Вывод my_container<int> с std::allocator ===\n";
    for (auto it = container1.begin(); it != container1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 8) создание экземпляра своего контейнера для хранения значений типа
    // int с новым аллокатором, ограниченным 10 элементами;
    std::cout << "\n=== 7. my_container<int> с my_allocator<int, 10> ===\n";
    my_container<int, my_allocator<int, 10>> container2;

    // 9) заполнение 10 элементами от 0 до 9;
    for (int i = 0; i < 10; ++i) {
        container2.push_back(i);
    }

    // 10) вывод на экран всех значений, хранящихся в контейнере.
    std::cout << "=== 8. Вывод my_container<int> с my_allocator ===\n";
    for (auto it = container2.begin(); it != container2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    return 0;
}