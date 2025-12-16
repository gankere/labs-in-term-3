#include "Parser.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>      // добавлено: требуется для std::vector
#include <string>      // добавлено: требуется для std::string
#include <algorithm>   // добавлено: требуется для std::transform
#include <cctype>      // добавлено: требуется для std::tolower

namespace po = boost::program_options;

Options Parser::parse(int argc, char** argv) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("dirs",
        po::value<std::vector<std::string>>()->multitoken()->required(),
        "Directories to scan (required, multiple allowed)")
        ("exclude",
        po::value<std::vector<std::string>>()->multitoken(),
        "Directories to exclude from scanning (multiple allowed)")
        ("depth",
        po::value<int>()->default_value(-1),
        "Recursion depth (0 = current dir only, -1 = unlimited)")
        ("min-size",
        po::value<size_t>()->default_value(1),
        "Minimum file size in bytes (default: 1)")
        ("mask",
        po::value<std::vector<std::string>>()->multitoken(),
        "File name masks (case-insensitive, e.g. *.txt *.log)")
        ("block-size",
        po::value<size_t>()->default_value(4096),
        "Block size for reading files (default: 4096)")
        ("hash",
        po::value<std::string>()->default_value("crc32"),
        "Hash algorithm (only crc32 is supported)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        std::exit(0);
    }
    po::notify(vm); //проверка выполненных требований

    Options opts; //создали контейнер
    opts.dirs = vm["dirs"].as<std::vector<std::string>>(); //Сохраняем список включённых директорий
    if (vm.count("exclude")) {
        opts.exclude_dirs = vm["exclude"].as<std::vector<std::string>>(); //Сохраняем список исключённых директорий
    }
    opts.depth = vm["depth"].as<int>(); //Сохраняем глубину
    opts.min_size = vm["min-size"].as<size_t>(); //Сохраняем минимальный размер
    if (vm.count("mask")) {
        opts.masks = vm["mask"].as<std::vector<std::string>>();
    }
    opts.block_size = vm["block-size"].as<size_t>(); //Сохраняем размер блока для чтения файлов

    std::string hash = vm["hash"].as<std::string>(); //Выбор алгоритма хеширования
    std::transform(hash.begin(), hash.end(), hash.begin(), [](unsigned char c) { return std::tolower(c); });
    if (hash != "crc32" && hash != "md5") {
        throw std::invalid_argument("Hash must be 'crc32' or 'md5'");
    }
    opts.hash_algo = hash;

    if (opts.block_size == 0) {
        throw std::invalid_argument("Block size must be > 0");
    }

    return opts; //возврат объекта с настройками
}