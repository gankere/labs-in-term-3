#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <cstddef>
#include <new>
#include <vector>
#include <iostream>

template <class T>
class my_allocator {
private:
    size_t capacity; //максимальная вместимость пула
    size_t curIndex; //индекс текущего свободный элемент
    size_t blockSize; //размер пула
    T* curBlock = nullptr; //указатель на текущий пул
    std::vector<T*> freeSlots; // вектор их ранее освобождённых слотов 

    size_t total_allocated_bytes = 0; // всего байт выделено

    // Структура для отслеживания блока
    struct BlockInfo {
        T* block_ptr;
        std::vector<bool> is_free; // true = свободен, false = занят
    };
    std::vector<BlockInfo> blocks_info; // Пул зарезервированных блоков и их состояния

    void expand() {
        T* new_block = static_cast<T*>(::operator new(blockSize * sizeof(T)));
        
        BlockInfo info;
        info.block_ptr = new_block;
        info.is_free.resize(blockSize, true);
        
        blocks_info.push_back(info);
        
        curBlock = new_block;
        curIndex = 0;
        capacity = blockSize;

        // Увеличиваем статистику
        total_allocated_bytes += blockSize * sizeof(T);
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
    struct rebind { // rebind — позволяет использовать аллокатор с другим типом
        using other = my_allocator<U>;
    };

    void clear() {
        // Освобождаем все блоки
        for (auto& block_info : blocks_info) {
            ::operator delete(block_info.block_ptr);
        }
        blocks_info.clear();
        freeSlots.clear();
        curBlock = nullptr;
        curIndex = 0;
        capacity = 0;
        total_allocated_bytes = 0; // сбрасываем статистику
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

            if (curBlock == nullptr || curIndex >= capacity) {
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
            freeSlots.push_back(p);  // положить в пул
        } else {
            ::operator delete(p);    // обычное удаление
        }
    }

    template <class U, class... Args>
    void construct(U* ptr, Args&&... args) {
        ::new ((void*)ptr) U(std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* ptr) {
        ptr->~U();
    }

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