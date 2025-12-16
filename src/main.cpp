#include "Parser.h"
#include "FileScanner.h"
#include "DuplicateFinder.h"
#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

void print_result(const std::vector<std::vector<fs::path>>& groups) {
    for (const auto& group : groups) {
        if (group.size() < 2) continue; // дубликаты — минимум 2 файла
        for (const auto& path : group) {
            std::cout << path.string() << '\n';
        }
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]) {
    try {
        Parser parser;
        Options opts = parser.parse(argc, argv);
        auto files = FileScanner::scan(opts);
        auto groups = DuplicateFinder::find_duplicates(files, opts);
        print_result(groups);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}