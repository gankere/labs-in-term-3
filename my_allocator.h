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
    std::vector<T*> memory_pool;    //пул зарезервированных блоков
    std::vector<T*> free_list;      //список свободных слотов

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

    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;

        //для множества элементов
        if (n > 1) { //запрашивают 2+ эл-в за раз
            return static_cast<T*>(::operator new(n * sizeof(T))); //обычное выделение
        }

        //для единичных элементов
        if (free_list.empty()) { //если нет свободных слотов
            T* block = static_cast<T*>(::operator new(N * sizeof(T))); //выделяем память под новый блок из N элементов
            memory_pool.push_back(block); //сохраняем указатель на блок

            for (std::size_t i = 0; i < N; ++i) {
                free_list.push_back(block + i); //в free_list "новые" свободные ячейки
            }
        }
        T* ptr = free_list.back(); //берём свободную ячейку с конца
        free_list.pop_back(); // минусуем эту ячейку
        return ptr;
    }

    void deallocate(T* p, std::size_t n) {
        //освобождение одиночного элемента
        if (n == 1) {
            free_list.push_back(p); //возврат указателя в свободные слоты
            try_free_empty_blocks(); //если блок стал свободным - освобождаем (что освобождаем?)
        } else { 
        //большие блоки освобождаем сразу
            ::operator delete(p);
        }
    }
   
    void try_free_empty_blocks() { //освобождение пустых блоков
        if (free_list.size() < N) return; //свободных слотов меньше N? -> нет пустогоблока

        //поиск пустых блоков
        for (auto it = memory_pool.begin(); it != memory_pool.end(); ) {
            T* block = *it;
            std::size_t free_count = 0;

            // Считаем сколько элементов этого блока в free_list
            for (T* elem : free_list) {
                if (elem >= block && elem < block + N) {
                    free_count++;
                }
            }

           
            if (free_count == N) { //все элементы блока свободны - освобождаем блок
                // Удаляем элементы этого блока из free_list
                free_list.erase(
                    std::remove_if(free_list.begin(), free_list.end(),
                        [block, this](T* elem) {
                            return elem >= block && elem < block + N;
                        }),
                    free_list.end()
                );

                // Освобождаем память и удаляем блок из пула
                ::operator delete(block);
                it = memory_pool.erase(it);
            } else {
                ++it;
            }
        }
    }

    void clear() { //полное освобождение памяти
        for (T* block : memory_pool) {
            ::operator delete(block); //удаляем памяти по указателю
        }
        memory_pool.clear(); //удаляем сами указатели
        free_list.clear(); //удаление указателей свободных ячеек
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