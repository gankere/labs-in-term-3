#include "FileScanner.h"
#include "Options.h"
#include <filesystem>
#include <algorithm>
#include <cctype>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

//подходит ли имя файла под glob маску
bool matches_mask(const std::string& filename, const std::string& mask) {
    if (mask.empty()) return true;

    auto it_file = filename.begin(); //текущий символ имени файла
    auto it_mask = mask.begin(); //текущий символ маски

    //проход по символам имени файла и маски
    while (it_file != filename.end() && it_mask != mask.end()) {
        if (*it_mask == '*') {
            ++it_mask; //переход к символу после '*'
            if (it_mask == mask.end()) return true;

            //поиск в имени файла символа, равного символу после '*' в маске
            while (it_file != filename.end() && *it_file != *it_mask) ++it_file;
            if (it_file == filename.end()) return false;
        } else if (*it_mask == '?') { //пропусаем по 1 символу
            ++it_file;
            ++it_mask;
        } else {
            if (std::tolower(*it_file) != std::tolower(*it_mask)) return false; //всё к нижнему регистру
            ++it_file;
            ++it_mask;
        }
    }

    return it_file == filename.end() && it_mask == mask.end(); //дошли до кнда имени файла и маски
}

bool is_valid_file(const fs::path& p, const Options& opts) {
    if (!fs::is_regular_file(p)) return false;

    if (fs::file_size(p) < opts.min_size) return false; //проверка мин размера

    // Маски имён
    if (!opts.masks.empty()) {
        std::string name = p.filename().string();
        bool matched = false;
        for (const auto& mask : opts.masks) {
            if (matches_mask(name, mask)) {
                matched = true;
                break;
            }
        }
        if (!matched) return false;
    }

    return true;
}

// Рекурсивный обход с учётом глубины и исключений
void scan_directory(
    const fs::path& root,
    const fs::path& current,
    int current_depth,
    const Options& opts,
    std::vector<fs::path>& result
) {
    if (opts.depth >= 0 && current_depth > opts.depth) { //не глубже установленной глубины
        return; 
    }

    try { //boost::filesystem::directory_iterator
        for (const auto& entry : fs::directory_iterator(current)) { 
            if (entry.is_directory()) {
                //директория не исключена?
                fs::path full_path = entry.path();
                bool excluded = false;

                // Проверка, не исключена ли эта директория
                for (const auto& exclude : opts.exclude_dirs) {
                    if (full_path.string().find(exclude) == 0) {
                        excluded = true;
                        break;}
                    }
                
                if (!excluded) {//если нет рекурсивный обход директории
                    scan_directory(root, full_path, current_depth + 1, opts, result);}

            } else if (entry.is_regular_file()) { //проверка валидности файла
                if (is_valid_file(entry.path(), opts)) {
                    result.push_back(entry.path());
                }
            }
        }
    } catch (...) {}
}

//Обработка всех директорий через --dirs
std::vector<fs::path> FileScanner::scan(const Options& opts) {
    std::vector<fs::path> files;
    //для каждой вызываем сканнер + 
    for (const auto& dir : opts.dirs) {
        fs::path root = dir;
        scan_directory(root, root, 0, opts, files); //все файлы в список
    }
    return files;
}