#include <iostream>
#include <stdexcept>
#include <memory>

//Последовательный контейнер с резервированием 
//[0][1][2][3][4][ ][ ][ ] элементы расположены подряд
template <typename T> //для работы с любым типом данных
class MySequentialContainer {
private:
    std::unique_ptr<T[]> data = nullptr;
    size_t current_size = 0; //кол-во элементов массива
    size_t capacity = 0; //размер выделенной памяти

    void check_index(size_t index) const {
        if (index >= current_size) throw std::out_of_range("MySequentialContainer: Index out of range");
    }

    void reserve_more() {
        size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
        std::unique_ptr<T[]> new_data(new T[new_capacity]);

        for (size_t i = 0; i < current_size; ++i) {
            new_data[i] = std::move(data[i]);
        }

        data = std::move(new_data);
        capacity = new_capacity;
    }

public:
    MySequentialContainer() = default;

    void push_back(const T& value) { //для копирования именованных объектов
        if (current_size >= capacity) {
            reserve_more();
        }
        data[current_size] = value;
        current_size++;
    }
    void push_back(T&& value) { //для перемещения временных объектов
            if (current_size >= capacity) {
                reserve_more();
            }
            data[current_size] = std::move(value);
            current_size++;
        }
    

    size_t size() const { return current_size; }

    T& operator[](size_t index) {
        check_index(index);
        return data[index];
    }
    const T& operator[](size_t index) const {
        check_index(index);
        return data[index];
    }

    void insert(size_t pos, const T&& value) {
        if (pos > current_size) throw std::out_of_range("Insert position out of range");

        if (current_size >= capacity) {
            reserve_more();
        }
        //Сдвигаем элементы вправо чтобы освободить место для нового элемента
        for (size_t i = current_size; i > pos; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[pos] = std::move(value);
        current_size++;
    }

    void erase(size_t pos) {
        if (pos >= current_size) throw std::out_of_range("Erase position out of range");
        //Сдвигаем элементы влево чтобы заполнить пустое место
        for (size_t i = pos; i < current_size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        current_size--;
    }
    //срабатывает, при создании нового контейнера
    MySequentialContainer(MySequentialContainer&& other) noexcept = default; //Перемещающий конструктор (генерируется автоматически)
    //срабатывает, при уже существующем контейнере
    MySequentialContainer& operator=(MySequentialContainer&& other) noexcept = default; //Перемещающий оператор присваивания (генерируется автоматически)

    // Копирование
    MySequentialContainer(const MySequentialContainer& other) //Копирующий конструктор
        : current_size(0), capacity(0) {
        if (other.current_size > 0) {
            capacity = other.current_size;
            data = std::unique_ptr<T[]>(new T[capacity]); //Выделяем память и копируем все элементы
            for (size_t i = 0; i < other.current_size; ++i) {
                data[i] = other.data[i];
            }
            current_size = other.current_size;
        }
    }

    MySequentialContainer& operator=(const MySequentialContainer& other) { //Копирующий оператор присваивания
        if (this != &other) { //Проверяем что это не присваивание самому себе
            if (other.current_size > 0) {
                capacity = other.current_size;
                data = std::unique_ptr<T[]>(new T[capacity]);
                for (size_t i = 0; i < other.current_size; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
                capacity = 0;
            }
            current_size = other.current_size;
        }
        return *this; //возвращаем ссылку на текущий объект
    }

    // Итераторы для последовательного доступа к элементам
    class iterator {
    private:
        T* ptr; //Создаём указатель на ячейку
    public:
        iterator(T* p) : ptr(p) {} //Конструктор: создаёт итератор, указывающий на массив p
        T& operator*() const { return *ptr; } //Оператор *: получаем значение ячейки из массива, на который указывает итератор
        iterator& operator++() { ++ptr; return *this; } //переход к следующией ячейке
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
    };

    class const_iterator {
    private:
        const T* ptr;
    public:
        const_iterator(const T* p) : ptr(p) {}
        const T& operator*() const { return *ptr; }
        const_iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
    };
    // Реализация итераторов
    iterator begin() { return iterator(data.get()); }
    iterator end() { return iterator(data.get() + current_size); }
    const_iterator begin() const { return const_iterator(data.get()); }
    const_iterator end() const { return const_iterator(data.get() + current_size); }

    void print() const {
        for (size_t i = 0; i < current_size; ++i) {
            std::cout << data[i];
            if (i < current_size - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

//Двунаправленный списковый контейнер
//[0] <-> [1] <-> [2] <-> [3] элементы разбросаны, связаны указателями
template <typename T>
class MyListContainer {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next; // указатель вперед
        Node* prev; // указатель назад

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    std::unique_ptr<Node> head;
    Node* tail;
    size_t current_size;

public:
    MyListContainer() : head(nullptr), tail(nullptr), current_size(0) {}

    void push_back(const T& value) {
        std::unique_ptr<Node> new_node(new Node(value)); //создали новый узел
        Node* new_node_ptr = new_node.get(); //получили указатель на узел

        if (!tail) {
            head = std::move(new_node);
            tail = new_node_ptr;
        } else {
            tail->next = std::move(new_node);
            tail->next->prev = tail;
            tail = tail->next.get();
        }
        current_size++;
    }

    size_t size() const { return current_size; }

    T& operator[](size_t index) {
        if (index >= current_size) throw std::out_of_range("Index out of range");
        
        Node* current = head.get();
        for (size_t i = 0; i < index; ++i) { //Нужно пройти все предыдущие узлы
            current = current->next.get();
        }
        return current->data;
    }
    const T& operator[](size_t index) const {
        if (index >= current_size) throw std::out_of_range("Index out of range");
        
        Node* current = head.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    void insert(size_t pos, const T& value) {
        if (pos > current_size) throw std::out_of_range("Insert position out of range");

        if (pos == current_size) {
            push_back(value);
            return;
        }

        if (pos == 0) {
            std::unique_ptr<Node> new_node(new Node(value));
            if (head) {
                new_node->next = std::move(head);
                new_node->next->prev = new_node.get();
            }
            head = std::move(new_node);
            if (!tail) tail = head.get();
            current_size++;
            return;
        }

        Node* current = head.get();// Находим узел перед позицией вставки
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next.get();
        }

        std::unique_ptr<Node> new_node(new Node(value));
        new_node->next = std::move(current->next);
        new_node->prev = current;
        
        if (new_node->next) {
            new_node->next->prev = new_node.get();
        }
        
        current->next = std::move(new_node);
        current_size++;
    }

void erase(size_t pos) {
    if (pos >= current_size) throw std::out_of_range("Erase position out of range");

    if (pos == 0) {
        if (head) {
            head = std::move(head->next);
            if (head) head->prev = nullptr;
            else tail = nullptr;
        }
        current_size--;
        return;
    }

    Node* prev_node = head.get();
    for (size_t i = 0; i < pos - 1; ++i) {
        prev_node = prev_node->next.get();
    }

    
    if (prev_node->next) {// Теперь prev_node - это узел перед удаляемым
        std::unique_ptr<Node> node_to_remove = std::move(prev_node->next);
        
        if (node_to_remove->next) {
            prev_node->next = std::move(node_to_remove->next);
            prev_node->next->prev = prev_node;
        } else {
            // Удаляемый узел был последним
            prev_node->next = nullptr;
            tail = prev_node;
        }
    }

    current_size--;
}

    MyListContainer(MyListContainer&& other) noexcept = default;
    MyListContainer& operator=(MyListContainer&& other) noexcept = default;

    //Копирование
    MyListContainer(const MyListContainer& other) : head(nullptr), tail(nullptr), current_size(0) {
        Node* current = other.head.get();
        while (current) {
            push_back(current->data);
            current = current->next.get();
        }
    }
    MyListContainer& operator=(const MyListContainer& other) {
        if (this != &other) {
            head = nullptr;
            tail = nullptr;
            current_size = 0;
            
            Node* current = other.head.get();
            while (current) {
                push_back(current->data);
                current = current->next.get();
            }
        }
        return *this;
    }

    //Итераторы
    class iterator {
    private:
        Node* ptr;
    public:
        iterator(Node* p) : ptr(p) {}
        T& operator*() const { return ptr->data; }
        iterator& operator++() { 
            if (ptr) ptr = ptr->next.get(); 
            return *this; 
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
    };

    class const_iterator {
    private:
        const Node* ptr;
    public:
        const_iterator(const Node* p) : ptr(p) {}
        const T& operator*() const { return ptr->data; }
        const_iterator& operator++() { 
            if (ptr) ptr = ptr->next.get(); 
            return *this; 
        }
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
    };

    iterator begin() { return iterator(head.get()); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head.get()); }
    const_iterator end() const { return const_iterator(nullptr); }

    void print() const {
        Node* current = head.get();
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << ", ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};

//Однонаправленный списковый контейнер
// [0] -> [1] -> [2] -> [3] -> nullptr только указатели вперед
template <typename T>
class MySingleListContainer {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
    size_t current_size;

public:
    MySingleListContainer() : head(nullptr), current_size(0) {}

    void push_back(const T& value) {
        std::unique_ptr<Node> new_node(new Node(value));
        
        if (!head) {
            head = std::move(new_node);
        } else {
            Node* current = head.get();
            while (current->next) {
                current = current->next.get();
            }
            current->next = std::move(new_node);
        }
        current_size++;
    }

    size_t size() const { return current_size; }

    T& operator[](size_t index) {
        if (index >= current_size) throw std::out_of_range("Index out of range");
        
        Node* current = head.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }
    const T& operator[](size_t index) const {
        if (index >= current_size) throw std::out_of_range("Index out of range");
        
        Node* current = head.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    void insert(size_t pos, const T& value) {
        if (pos > current_size) throw std::out_of_range("Insert position out of range");

        if (pos == 0) {
            std::unique_ptr<Node> new_node(new Node(value));
            new_node->next = std::move(head);
            head = std::move(new_node);
            current_size++;
            return;
        }

        if (pos == current_size) {
            push_back(value);
            return;
        }

        Node* current = head.get();
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next.get();
        }

        std::unique_ptr<Node> new_node(new Node(value));
        new_node->next = std::move(current->next);
        current->next = std::move(new_node);
        current_size++;
    }

    void erase(size_t pos) {
        if (pos >= current_size) throw std::out_of_range("Erase position out of range");

        if (pos == 0) {
            head = std::move(head->next);
            current_size--;
            return;
        }

        Node* current = head.get();
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next.get();
        }

        current->next = std::move(current->next->next);
        current_size--;
    }

    MySingleListContainer(MySingleListContainer&& other) noexcept = default;
    MySingleListContainer& operator=(MySingleListContainer&& other) noexcept = default;

    // Копирование
    MySingleListContainer(const MySingleListContainer& other) : head(nullptr), current_size(0) {
        Node* current = other.head.get();
        while (current) {
            push_back(current->data);
            current = current->next.get();
        }
    }
    MySingleListContainer& operator=(const MySingleListContainer& other) {
        if (this != &other) {
            head = nullptr;
            current_size = 0;
            
            Node* current = other.head.get();
            while (current) {
                push_back(current->data);
                current = current->next.get();
            }
        }
        return *this;
    }

    // Итераторы
    class iterator {
    private:
        Node* ptr;
    public:
        iterator(Node* p) : ptr(p) {}
        T& operator*() const { return ptr->data; }
        iterator& operator++() { 
            if (ptr) ptr = ptr->next.get(); 
            return *this; 
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
    };
    class const_iterator {
    private:
        const Node* ptr;
    public:
        const_iterator(const Node* p) : ptr(p) {}
        const T& operator*() const { return ptr->data; }
        const_iterator& operator++() { 
            if (ptr) ptr = ptr->next.get(); 
            return *this; 
        }
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
    };

    iterator begin() { return iterator(head.get()); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head.get()); }
    const_iterator end() const { return const_iterator(nullptr); }

    void print() const {
        Node* current = head.get();
        while (current) {
            std::cout << current->data;
            if (current->next) std::cout << ", ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};

//ДЕМОНСТРАЦИЯ ВОЗМОЖНОСТЕЙ
void demo_sequential() {
  std::cout << "=== ПОСЛЕДОВАТЕЛЬНЫЙ КОНТЕЙНЕР ===" << std::endl;
  MySequentialContainer<int> seq;

  for (int i = 0; i < 10; i++) {
    seq.push_back(i);
  }
  std::cout << "После добавления 0-9: ";
  seq.print();
  std::cout << "Размер: " << seq.size() << std::endl;

  std::cout << "seq[0] = " << seq[0] << std::endl;
  std::cout << "seq[5] = " << seq[5] << std::endl;
  seq[2] = 100;
  std::cout << "После seq[2] = 100: ";
  seq.print();

  seq.erase(2); 
  seq.erase(3);
  seq.erase(4);
  std::cout << "Минус 3й, 5й, 7й элемент: ";
  seq.print();

  seq.insert(0, 10);
  std::cout << "10 в начало: ";
  seq.print();

  seq.insert(4, 212);
  std::cout << "212 в середину: ";
  seq.print();

  seq.push_back(27);
  std::cout << "27 в конец: ";
  seq.print();

  std::cout << "Прямой обход: ";
  for (auto it = seq.begin(); it != seq.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  MySequentialContainer<int> seq_moved = std::move(seq);
  std::cout << "После перемещения - новый контейнер: ";
  seq_moved.print();
  std::cout << "Исходный контейнер размер: " << seq.size() << std::endl;

  MySequentialContainer<int> seq_copied = seq_moved;
  std::cout << "Копия контейнера: ";
  seq_copied.print();
  seq_copied[0] = 999;
  std::cout << "После изменения копии [0]=999:" << std::endl;
  std::cout << "Копия: ";
  seq_copied.print();
  std::cout << "Оригинал: ";
  seq_moved.print();

  seq_copied.push_back(42);
  std::cout << "После push_back(42): ";
  seq_copied.print();
}

void demo_doubly_linked() {
  std::cout << "\n=== ДВУНАПРАВЛЕННЫЙ СПИСОК ===" << std::endl;
  MyListContainer<int> dll;

  for (int i = 0; i < 10; i++) {
    dll.push_back(i);
  }
  std::cout << "После добавления 0-9: ";
  dll.print();
  std::cout << "Размер: " << dll.size() << std::endl;

  std::cout << "dll[0] = " << dll[0] << std::endl;
  std::cout << "dll[5] = " << dll[5] << std::endl;
  dll[2] = 200;
  std::cout << "После dll[2] = 200: ";
  dll.print();

  dll.erase(2); 
  dll.erase(3);
  dll.erase(4);
  std::cout << "Минус 3й, 5й, 7й элемент: ";
  dll.print();

  dll.insert(0, 10);
  std::cout << "10 в начало: ";
  dll.print();

  dll.insert(4, 241);
  std::cout << "241 в середину: ";
  dll.print();

  dll.push_back(34);
  std::cout << "34 в конец: ";
  dll.print();

  std::cout << "Прямой обход: ";
  for (auto it = dll.begin(); it != dll.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "Изменение через итераторы (умножить на 2): ";
  for (auto it = dll.begin(); it != dll.end(); ++it) {
    *it = *it * 2;
  }
  dll.print();

  MyListContainer<int> dll_moved = std::move(dll);
  std::cout << "После перемещения - новый контейнер: ";
  dll_moved.print();
  std::cout << "Исходный контейнер размер: " << dll.size() << std::endl;

  MyListContainer<int> dll_copied = dll_moved;
  std::cout << "Копия контейнера: ";
  dll_copied.print();
  dll_copied[0] = 777;
  std::cout << "После изменения копии [0]=777:" << std::endl;
  std::cout << "Копия: ";
  dll_copied.print();
  std::cout << "Оригинал: ";
  dll_moved.print();

  const MyListContainer<int> const_dll = dll_copied;
  std::cout << "Обход const контейнера: ";
  for (auto it = const_dll.begin(); it != const_dll.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

void demo_singly_linked() {
  std::cout << "\n=== ОДНОНАПРАВЛЕННЫЙ СПИСОК ===" << std::endl;
  MySingleListContainer<int> sll;

  for (int i = 0; i < 10; i++) {
    sll.push_back(i);
  }
  std::cout << "После добавления 0-9: ";
  sll.print();
  std::cout << "Размер: " << sll.size() << std::endl;

  std::cout << "sll[0] = " << sll[0] << std::endl;
  std::cout << "sll[5] = " << sll[5] << std::endl;
  sll[2] = 300;
  std::cout << "После sll[2] = 300: ";
  sll.print();

  sll.erase(2);
  sll.erase(3);
  sll.erase(4);
  std::cout << "Минус 3й, 5й, 7й элемент: ";
  sll.print();

  sll.insert(0, 10);
  std::cout << "10 в начало: ";
  sll.print();

  sll.insert(4, 51);
  std::cout << "51 в середину: ";
  sll.print();

  sll.push_back(31);
  std::cout << "31 в конец: ";
  sll.print();

  std::cout << "Прямой обход: ";
  for (auto it = sll.begin(); it != sll.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "Поиск элемента 51 через итераторы: ";
  for (auto it = sll.begin(); it != sll.end(); ++it) {
    if (*it == 51) {
      std::cout << "найден!";
      break;
    }
  }
  std::cout << std::endl;

  MySingleListContainer<int> sll_moved = std::move(sll);
  std::cout << "После перемещения - новый контейнер: ";
  sll_moved.print();
  std::cout << "Исходный контейнер размер: " << sll.size() << std::endl;

  MySingleListContainer<int> sll_copied = sll_moved;
  std::cout << "Копия контейнера: ";
  sll_copied.print();
  sll_copied[0] = 888;
  std::cout << "После изменения копии [0]=888:" << std::endl;
  std::cout << "Копия: ";
  sll_copied.print();
  std::cout << "Оригинал: ";
  sll_moved.print();

  std::cout << "Доступ по индексу sll[7]: " << sll_copied[7] << std::endl;
  std::cout << "Доступ через итераторы до 7-го элемента: ";
  auto it = sll_copied.begin();
  for (int i = 0; i < 7; ++i) ++it;
  std::cout << *it << std::endl;
}

int main() {
  demo_sequential();
  demo_doubly_linked();
  demo_singly_linked();
  return 0;
}