#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <cstddef>
#include <new>
#include <vector>
#include <algorithm> // для std::remove_if
#include <limits>    // для max_size
template <class T, std::size_t N = 5> //N - сколько выделить
class my_allocator {
private:
    // Структура для отслеживания блока
    struct BlockInfo {
        T* block_ptr;
        std::vector<bool> is_free; // true = свободен, false = занят
    };

    std::vector<BlockInfo> blocks_info; // Пул зарезервированных блоков и их состояния

public:
    //переименование типов
    using value_type = T; //тип, который храним
    using pointer = T*; //указатель
    using const_pointer = const T*; //константный указатель
    using reference = T&; //ссылка
    using const_reference = const T&; //константная ссылка
    using size_type = std::size_t; //тип размерности (сколько элементов в контейнере)
    using difference_type = std::ptrdiff_t; //тип разницы указателей

    template <class U> //создание аллокатора типа U по типу прототипа T
    struct rebind {
        using other = my_allocator<U, N>;
    };

    my_allocator() = default;

    ~my_allocator() {
        clear();
    }

// линейный пример
    // T* allocate (std::size_t n){ //принимаем кол-во элементов
    //     std::size_t bytes = n * sizeof(T); //сколько байт выделяем
    //     auto return_memory = current;
    //     current += bytes; //сдвигаем указатель
    //     return reinterpret_cast<T*>(return_memory); //внутри пула работаем с сырой паматью, поэтому меняем тип указателя на T*
    // }

    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;

        //для множества элементов
        if (n > 1) { //запрашивают 2+ эл-в за раз
            return static_cast<T*>(::operator new(n * sizeof(T))); //обычное выделение
        }

        //для единичных элементов
        // Ищем свободный слот в уже выделенных блоках
        for (auto& block_info : blocks_info) {
            for (std::size_t i = 0; i < N; ++i) {
                if (block_info.is_free[i]) {
                    block_info.is_free[i] = false; // помечаем как занятый
                    return block_info.block_ptr + i; // возвращаем указатель
                }
            }
        }

        //если нет свободных слотов, выделяем новый блок
        T* block = static_cast<T*>(::operator new(N * sizeof(T))); //выделяем память под новый блок из N элементов
        blocks_info.push_back({block, std::vector<bool>(N, true)}); //сохраняем указатель на блок и вектор занятости (все свободны)

        // Помечаем первый слот как занятый
        blocks_info.back().is_free[0] = false;
        return block; //возвращаем указатель на первый элемент нового блока
    }

    void deallocate(T* p, std::size_t n) {
        //освобождение одиночного элемента
        if (n == 1) {
            // Найти, какому блоку принадлежит p
            for (auto& block_info : blocks_info) {
                if (p >= block_info.block_ptr && p < block_info.block_ptr + N) {
                    std::size_t idx = p - block_info.block_ptr; // вычисляем индекс
                    block_info.is_free[idx] = true; //возврат указателя в свободные слоты
                    break; // выходим из цикла, нашли
                }
            }
            try_free_empty_blocks(); //если блок стал свободным - освобождаем
        } else {
        //большие блоки освобождаем сразу
            ::operator delete(p);
        }
    }

    void try_free_empty_blocks() { //освобождение пустых блоков
        // Цикл по блокам (итератор нужен для erase)
        for (auto it = blocks_info.begin(); it != blocks_info.end(); ) {
            // Проверяем, все ли слоты в блоке свободны
            bool all_free = true;
            for (bool is_slot_free : it->is_free) {
                if (!is_slot_free) {
                    all_free = false;
                    break;
                }
            }

            if (all_free) { //все элементы блока свободны - освобождаем блок
                // Освобождаем память и удаляем блок из пула
                ::operator delete(it->block_ptr);
                it = blocks_info.erase(it); // erase возвращает итератор на следующий элемент
            } else {
                ++it; // переходим к следующему блоку
            }
        }
    }

    void clear() { //полное освобождение памяти
        for (const auto& block_info : blocks_info) {
            ::operator delete(block_info.block_ptr); //удаляем памяти по указателю
        }
        blocks_info.clear(); //удаляем сами блоки
    }

    ///////////////////////////////////////////////////////////////
    template <class U> //шаблон: можно работать с любым типомданных U
    my_allocator(const my_allocator<U, N>&) noexcept {}//создаём аллокатор на основе U

    template <class U>
    bool operator==(const my_allocator<U, N>&) const noexcept {
        return true;
    }

    template <class U>
    bool operator!=(const my_allocator<U, N>& other) const noexcept {
        return !(*this == other);
    }
    ///////////////////////////////////////////////////////////////
};

#endif // MY_ALLOCATOR_H