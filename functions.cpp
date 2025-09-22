#include "functions.h"

ip_record parseLine(const std::string& line){
    size_t first_tab = line.find('\t');
    size_t second_tab = line.find('\t', first_tab+1);

    std::string ip_str = line.substr(0, first_tab);
    std::string text2 = line.substr(first_tab + 1, second_tab - first_tab - 1);
    std::string text3 = line.substr(second_tab + 1);

    std::stringstream ss(ip_str);
    std::string part;
    int ip_parts[4];

    for (int i = 0; i < 4; ++i){
        std::getline(ss, part, '.');
        ip_parts[i] = std::stoi(part);
    }

    ip_address ip_tuple = std::make_tuple(ip_parts[0], ip_parts[1], ip_parts[2], ip_parts[3]);
    return std::make_tuple(ip_tuple, text2, text3);
}

std::vector<ip_record> readAllLines (const std::string& filename){
    std::vector<ip_record> records;
    std::ifstream file(filename);

    std::string line;
    while (std::getline(file,line)){
        ip_record record = parseLine(line);
        records.push_back(record);
    }
    return records;
}

void printRecord(const ip_record& record){
    const ip_address& ip = std::get<0>(record);
    const std::string& text2 = std::get<1>(record);
    const std::string& text3 = std::get<2>(record);

    std::cout << std::get<0>(ip) << "."
              << std::get<1>(ip) << "."
              << std::get<2>(ip) << "."
              << std::get<3>(ip) << "\t"
              << text2 << "\t"
              << text3 << std::endl;
}

void mainSort(std::vector<ip_record>& ip_pool) {
    std::sort(ip_pool.begin(), ip_pool.end(), [](const ip_record& a, const ip_record& b) {
        return std::get<0>(a) > std::get<0>(b);
    });
}

void printFullList(const std::vector<ip_record>& ip_pool) {
    std::cout << "\t--- Полный отсортированный список ---\n";
    for (const auto& record : ip_pool) {
        printRecord(record);
    }
}

void printWith1(const std::vector<ip_record>& ip_pool) {
    std::cout << "\t--- Первый байт == 1 ---\n";
    for (const auto& record : ip_pool) {
        const ip_address& ip = std::get<0>(record);
        if (std::get<0>(ip) == 1) {
            printRecord(record);
        }
    }
}

void printWith46And70(const std::vector<ip_record>& ip_pool) {
    std::cout << "\t--- Первый байт == 46 && Второй байт == 70 ---\n";
    for (const auto& record : ip_pool) {
        const ip_address& ip = std::get<0>(record);
        if (std::get<0>(ip) == 46 && std::get<1>(ip) == 70) {
            printRecord(record);
        }
    }
}

void printWith46(const std::vector<ip_record>& ip_pool) {
    std::cout << "\t--- Любой байт == 46 ---\n";
    for (const auto& record : ip_pool) {
        const ip_address& ip = std::get<0>(record);
        if (std::get<0>(ip) == 46 ||
            std::get<1>(ip) == 46 ||
            std::get<2>(ip) == 46 ||
            std::get<3>(ip) == 46) {
            printRecord(record);
        }
    }
}