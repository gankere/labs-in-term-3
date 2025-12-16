#include "DuplicateFinder.h"
#include "Hasher.h"

#include <fstream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

// Ленивый хешер файла
class LazyFile {
    fs::path path_;
    size_t block_size_;
    std::unique_ptr<Hasher> hasher_;

    mutable std::vector<std::string> cached_hashes_; //кеш вычисленных блоков
    mutable size_t file_size_ = 0; //кеш файла
    mutable bool size_cached_ = false; //был закеширован?

public:
    LazyFile(fs::path p, size_t bs, std::unique_ptr<Hasher> h)
        : path_(std::move(p)), block_size_(bs), hasher_(std::move(h)) {}
    
    //с помощью кеша исбегаем считывание файла более 1 раза
    size_t get_file_size() const { 
        if (!size_cached_) {
            file_size_ = fs::file_size(path_); //читаем размер с диска
            size_cached_ = true;
        }
        return file_size_;
    }

    //вычисление кол-ва блоков в файле
    size_t block_count() const {
        size_t s = get_file_size();
        return (s + block_size_ - 1) / block_size_;
    }

    std::string get_block_hash(size_t index) const {
        if (index >= cached_hashes_.size()) {
            cached_hashes_.resize(index + 1);
        }
        std::string& cache = cached_hashes_[index];
        if (cache.empty()) {
            std::ifstream file(path_.string(), std::ios::binary); //Открытие файла в бинарном режиме
            file.seekg(static_cast<std::streamoff>(index * block_size_));
            std::vector<char> buffer(block_size_, 0); //в начале блока создание буфера размера S, заполненный нулями
            file.read(buffer.data(), static_cast<std::streamsize>(block_size_));
            size_t bytes_read = static_cast<size_t>(file.gcount());
            std::fill(buffer.begin() + bytes_read, buffer.end(), 0);
            cache = hasher_->hash(buffer.data(), block_size_);
        }
        return cache;
    }

    const fs::path& path() const { return path_; }
};

bool are_equal(const LazyFile& a, const LazyFile& b) {
    // Если размеры разные — файлы точно не равны
    if (a.get_file_size() != b.get_file_size()) return false;
    size_t n = a.block_count();
    //Сравнение блоков по-одному
    for (size_t i = 0; i < n; ++i) {
        if (a.get_block_hash(i) != b.get_block_hash(i))
            return false;
    }
    return true;
}

std::vector<std::vector<fs::path>> DuplicateFinder::find_duplicates(
    const std::vector<fs::path>& files,
    const Options& opts
) {
    //Создание хешера
        std::unique_ptr<Hasher> base_hasher;
    if (opts.hash_algo == "crc32") {
        base_hasher = std::make_unique<CRC32Hasher>();
    } else if (opts.hash_algo == "md5") {
        base_hasher = std::make_unique<MD5Hasher>();
    } else {
        throw std::runtime_error("Unsupported hash algorithm");
    }

    //Группировка по размеру
    std::unordered_map<size_t, std::vector<fs::path>> by_size;
    for (const auto& f : files) {
        try {
            by_size[fs::file_size(f)].push_back(f);
        } catch (...) {
            // Игнорируем недоступные файлы
        }
    }

    //Ленивое сравнение внутри групп
    std::vector<std::vector<fs::path>> result;
    for (auto& kv : by_size) {
        auto& group = kv.second;
        if (group.size() < 2) continue; // нет смысла сравнивать один файл

        // Оборачиваем файлы в LazyFile с выбранным хешером
        std::vector<LazyFile> lazy_files;
        for (const auto& path : group) {
            lazy_files.emplace_back(path, opts.block_size, base_hasher->clone());
        }

        //группировка дубликатов
        std::vector<bool> used(lazy_files.size(), false);
        for (size_t i = 0; i < lazy_files.size(); ++i) {
            if (used[i]) continue; //уже найденные дубликаты пропускаем
            std::vector<fs::path> current_group = {lazy_files[i].path()};
            //Сравнение с остальными файлами группы
            for (size_t j = i + 1; j < lazy_files.size(); ++j) {
                if (!used[j] && are_equal(lazy_files[i], lazy_files[j])) {
                    used[j] = true;
                    current_group.push_back(lazy_files[j].path());
                }
            }
            //если хотя бы 2 дубликаата в группе - сохраняем
            if (current_group.size() >= 2) {
                result.push_back(std::move(current_group));
            }
        }
    }

    return result;
}