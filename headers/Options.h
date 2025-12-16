#pragma once
#include <vector>
#include <string>

struct Options {
    std::vector<std::string> dirs;  //директории для сканирования
    std::vector<std::string> exclude_dirs; //исключённые директории
    int depth = -1;  //уровень сканирования (без ограничения глубины)
    size_t min_size = 1;  //минимальный размер файла (в байтах)
    std::vector<std::string> masks; //маски имён файлов
    size_t block_size = 512;
    std::string hash_algo = "crc32";
};