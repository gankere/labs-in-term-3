#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <cstddef>
#include <new> // для placement new

template <typename T, class Allocator = std::allocator<T>>
class my_container {
private:
    struct Node
    {
        Node *prev;
        Node *next;
        T value;
    };

    // Создаём аллокатор для Node
    typename Allocator::template rebind<Node>::other alloc;

    Node* head = nullptr;    // первый элемент
    Node* tail = nullptr;    // последний элемент
    size_t count = 0;        // количество элементов

public:

    // Простой итератор для обхода в одном направлении
    class iterator {
    private:
        Node* current;    // текущий узел

    public:
        // Конструктор
        iterator(Node* node = nullptr) : current(node) {}

        // Получить данные
        T& operator*() { return current->value; }  // ❗️ исправлено: data → value

        // Перейти к следующему элементу
        iterator& operator++() {
            current = current->next;
            return *this;
        }

        // Проверка на равенство
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    // Конструктор
    my_container() = default;

    // Деструктор - освобождаем всю память
    ~my_container() {
        clear();
    }

    // Добавить элемент в конец
    void push_back(const T& value) {
        // Выделяем память для нового узла через аллокатор
        Node* new_node = alloc.allocate(1);

        // Создаем узел в выделенной памяти (placement new)
        new (new_node) Node{nullptr, nullptr, value};  // ❗️ исправлено: Node(value) → Node{...}

        // Добавляем в список
        if (head == nullptr) {
            // Если список пустой - новый узел становится первым и последним
            head = new_node;
            tail = new_node;
        } else {
            // Если есть элементы - добавляем в конец
            tail->next = new_node;
            tail = new_node;
        }

        count++;
    }

    // Удалить первый элемент
    void pop_front() {
        if (head == nullptr) return; // если список пустой - ничего не делаем

        Node* to_delete = head;      // запоминаем первый элемент
        head = head->next;           // первым становится следующий элемент

        // Уничтожаем узел и освобождаем память
        to_delete->~Node();
        alloc.deallocate(to_delete, 1);

        count--;

        // Если список стал пустым - обновляем tail
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    // Очистить весь контейнер
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // Получить количество элементов
    size_t size() const {
        return count;
    }

    // Проверить пустой ли контейнер
    bool empty() const {
        return count == 0;
    }

    // Получить первый элемент
    T& front() {
        return head->value;  // ❗️ исправлено: data → value
    }

    // Получить последний элемент
    T& back() {
        return tail->value;  // ❗️ исправлено: data → value
    }

    // Итератор на начало
    iterator begin() {
        return iterator(head);
    }

    // Итератор на конец (после последнего элемента)
    iterator end() {
        return iterator(nullptr);
    }
    
    // В public: вашей my_container
    Allocator get_allocator() const {
        return alloc;  // возвращаем аллокатор
    }
};

#endif // MY_CONTAINER_H