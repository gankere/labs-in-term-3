#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <memory>  // для std::allocator_traits

template <class T, class Alloc = std::allocator<T>>
class my_list {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using traits = std::allocator_traits<allocator_type>;

    Node* head = nullptr;
    allocator_type alloc;
    std::size_t _size = 0;

public:
    explicit my_list(const Alloc& allocator = Alloc{}) : alloc(allocator) {}

    ~my_list() {
        clear();
    }

    // Добавить элемент в начало
    void push_front(const T& value) {
        Node* new_node = traits::allocate(alloc, 1);
        traits::construct(alloc, new_node, value);
        new_node->next = head;
        head = new_node;
        ++_size;
    }

    // Обход: напечатать все элементы
    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << "\n";
    }

    // Вспомогательные методы
    std::size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

    void clear() {
        while (head) {
            Node* next = head->next;
            traits::destroy(alloc, head);
            traits::deallocate(alloc, head, 1);
            head = next;
        }
        _size = 0;
    }

    // Итератор (простой)
    struct iterator {
        Node* ptr;

        iterator(Node* p = nullptr) : ptr(p) {}

        T& operator*() { return ptr->data; }
        iterator& operator++() { ptr = ptr->next; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

#endif // MY_CONTAINER_H