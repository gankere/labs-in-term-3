#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <cstddef>
#include <new>

template <typename T, class Allocator = std::allocator<T>>
class my_container {
private:
    //Структура узла для двусвязного списка
    struct Node 
    {
        Node *prev;
        Node *next;
        T value;
    };
  
    typename Allocator::template rebind<Node>::other alloc; //создание аллокатора для Node

    Node* head = nullptr;    //первый элемент
    Node* tail = nullptr;    //последний элемент
    size_t count = 0;        //количество элементов

public:
    //Итератор обхода в одном направлении
    class iterator {    
    private:
        Node* current;    // текущий узел

    public:
        iterator(Node* node = nullptr) : current(node) {} //создаёт итератор, указывающий на узел node (или на nullptr)

        T& operator*() { return current->value; } //получание данных через указатель

        iterator& operator++() {//переход к следующему элементу
            current = current->next;
            return *this;
        }

        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    my_container() = default;

    ~my_container() {
        clear();
    }

    //Добавление в конец
    void push_back(const T& value) {
        Node* new_node = alloc.allocate(1); //выделение памяти через аллокатор

        new (new_node) Node{nullptr, nullptr, value}; //создание узла в памяти

        if (head == nullptr) {
            //список пуст: новый узел - первый и последний
            head = new_node;
            tail = new_node;
        } else {
            //добавить узел в конец
            tail->next = new_node;  //связать последний с новым
            tail = new_node;        //обновить указатель на последний
        }

        count++;
    }

    //Удаление первого
    void pop_front() { 
        if (head == nullptr) return; //список пустой - ничего не делаем

        Node* to_delete = head;      // запоминили первый элемент
        head = head->next;           // первый становится следующим

        to_delete->~Node(); //удаление узла
        alloc.deallocate(to_delete, 1); //освобождение памяти

        count--;

        if (head == nullptr) {//список стал пустым - обновляем tail
            tail = nullptr;
        }
    }

    //Полная очистка
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    size_t size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    T& front() {
        return head->value;
    }

    T& back() {
        return tail->value;
    }

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
    
    Allocator get_allocator() const {
        return alloc;  // возвращаем аллокатор
    }
};

#endif // MY_CONTAINER_H