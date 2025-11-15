#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <cstddef>
#include <new>
#include <vector>
#include <iostream>

template <class T>
class my_allocator {
private:
    size_t capacity; //предел использования curBlock
    size_t curIndex; //текущий индекс
    size_t blockSize; //сколько элементов выделяется за раз
    T* curBlock = nullptr; //текущий пул
    std::vector<T*> freeSlots; // вектор из ранее освобождённых слотов 

    //для отслеживания общей выделенной памяти
    size_t total_allocated_bytes = 0; //сколько всего байт выделено

    
    struct BlockInfo { //отслеживание состояния блоак
        T* block_ptr; //отслеживаем текущий блок
        std::vector<bool> is_free; // true = свободен, false = занят
    };
    std::vector<BlockInfo> blocks_info; //вектор состояния выделенных блоков

    void expand() {
        T* new_block = static_cast<T*>(::operator new(blockSize * sizeof(T))); //указатель на новый выделенный блок памяти для blockSize элементов T.
        
        BlockInfo info; //сохдаём объект для хранения информации о новом блоке
        info.block_ptr = new_block; //указатель на ранее выделенный блок памяти
        info.is_free.resize(blockSize, true); //уст. blockSize элементов в векторе — каждый = true, 
        
        blocks_info.push_back(info); //запоминаем информацию о новом блоке
        
        curBlock = new_block;
        curIndex = 0;
        capacity = blockSize;

        total_allocated_bytes += blockSize * sizeof(T); //было выделено байт
    }

public:
    using value_type = T; //тип, который храним
    using pointer = T*; //указатель
    using const_pointer = const T*; //константный указатель
    using reference = T&; //ссылка
    using const_reference = const T&; //константная ссылка
    using size_type = std::size_t; //тип размерности (сколько элементов в контейнере)
    using difference_type = std::ptrdiff_t; //тип разницы указателей

    //конструктор с параметром кол-ва выделяемых за раз элементов
    explicit my_allocator(size_t Blocksize = 1) 
        : capacity(0), curIndex(0), blockSize(Blocksize ? Blocksize : 1), curBlock(nullptr) {}

    // конструктор копирования для разных типов
    template<typename U>
    my_allocator(const my_allocator<U>& other)
        : capacity(0), curIndex(0), blockSize(other.getBlockSize()), curBlock(nullptr) {}
        
    ~my_allocator() {
        clear();
    }

    template <class U>
    struct rebind {     //rebind — позволяет использовать аллокатор с другим типом (U)
        using other = my_allocator<U>;
    };

    void clear() {
        //освобожнение всех блоков памяти
        for (auto& block_info : blocks_info) {
            ::operator delete(block_info.block_ptr); //возврат памяти  системе 
        }
        blocks_info.clear();
        freeSlots.clear();
        curBlock = nullptr;
        curIndex = 0;
        capacity = 0;
        total_allocated_bytes = 0; //сброс статистики
        std::cout << "Cleared. Current allocated bytes: " << total_allocated_bytes << "\n";
    }

    T* allocate (std::size_t n){
        if (n == 0) return nullptr;

        if (n == 1){
            if (!freeSlots.empty()){
                T* ptr = freeSlots.back(); //берём последний указатель
                freeSlots.pop_back();
                return ptr;
            }

            if (curBlock == nullptr || curIndex >= capacity) { //текущий блок пустой или он заполнен
                expand();
                std::cout << "Expanded block. Current allocated bytes: " << total_allocated_bytes << "\n";
            }

            T* ptr = curBlock + curIndex;
            ++curIndex;
            return ptr;
        }

        // Для n > 1 — обычное выделение
        T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));
        total_allocated_bytes += n * sizeof(T); // добавляем к статистике
        std::cout << "Allocated " << n << " elements. Current allocated bytes: " << total_allocated_bytes << "\n";
        return ptr;
    }

    void deallocate(T* p, std::size_t n) noexcept {
        if (n == 1) {
            freeSlots.push_back(p);  //положить в пул
        } else {
            ::operator delete(p);    //освобождение сырого блока памяти, выделенного через ::operator new
        }
    }

    template <class U, class... Args> // Args... - пакет типов аргументов переменной длины
    void construct(U* ptr, Args&&... args) {
        //не выделяем новую память, а конструируем из той, что есть
        //((void*)ptr) - адрес, куда нужно поместить объект
        //std::forward<Args>(args)... - аргументы U, где Args...- пакет типов, а args... — пакет значений.
        ::new ((void*)ptr) U(std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* ptr) {
        ptr->~U();//освобождает ресурсы, принадлежащие объекту U
    }//выделенный блок памяти не освобождается

    size_t getBlockSize() const {
        return blockSize;
    }

    // Метод для получения общей выделенной памяти
    size_t allocated() const {
        return total_allocated_bytes;
    }

    bool operator==(const my_allocator&) const noexcept { return true; }
    bool operator!=(const my_allocator& other) const noexcept { return !(*this == other); }
};

#endif // MY_ALLOCATOR_H