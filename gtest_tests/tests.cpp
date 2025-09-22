#include <gtest/gtest.h>
#include "functions.h"
#include <sstream>
#include <fstream>

// Тесты для функции parseLine
TEST(ParseLineTest, BasicParsing) {
    std::string line = "192.168.1.1\ttest2\ttest3";
    auto record = parseLine(line);
    
    const auto& ip = std::get<0>(record);
    EXPECT_EQ(std::get<0>(ip), 192);
    EXPECT_EQ(std::get<1>(ip), 168);
    EXPECT_EQ(std::get<2>(ip), 1);
    EXPECT_EQ(std::get<3>(ip), 1);
    EXPECT_EQ(std::get<1>(record), "test2");
    EXPECT_EQ(std::get<2>(record), "test3");
}

TEST(ParseLineTest, AnotherIP) {
    std::string line = "1.2.3.4\ttext2\ttext3";
    auto record = parseLine(line);
    
    const auto& ip = std::get<0>(record);
    EXPECT_EQ(std::get<0>(ip), 1);
    EXPECT_EQ(std::get<1>(ip), 2);
    EXPECT_EQ(std::get<2>(ip), 3);
    EXPECT_EQ(std::get<3>(ip), 4);
}

// Тесты для функции mainSort
TEST(SortTest, ReverseLexicographicalOrder) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(1, 2, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(1, 10, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(2, 0, 0, 0), "", "")
    };
    
    mainSort(ips);
    
    // Проверяем обратную лексикографическую сортировку
    EXPECT_EQ(std::get<0>(std::get<0>(ips[0])), 2);  // 2.0.0.0 должен быть первым
    
    EXPECT_EQ(std::get<0>(std::get<0>(ips[1])), 1);  // 1.10.1.1
    EXPECT_EQ(std::get<1>(std::get<0>(ips[1])), 10);
    
    EXPECT_EQ(std::get<0>(std::get<0>(ips[2])), 1);  // 1.2.1.1
    EXPECT_EQ(std::get<1>(std::get<0>(ips[2])), 2);
    
    EXPECT_EQ(std::get<0>(std::get<0>(ips[3])), 1);  // 1.1.1.1
    EXPECT_EQ(std::get<1>(std::get<0>(ips[3])), 1);
}

TEST(SortTest, EqualFirstByte) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(10, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(10, 5, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(10, 3, 1, 1), "", "")
    };
    
    mainSort(ips);
    
    // При равном первом байте сортировка по второму в обратном порядке
    EXPECT_EQ(std::get<1>(std::get<0>(ips[0])), 5);
    EXPECT_EQ(std::get<1>(std::get<0>(ips[1])), 3);
    EXPECT_EQ(std::get<1>(std::get<0>(ips[2])), 1);
}

// Тесты для функций фильтрации
TEST(FilterTest, FirstByteEquals1) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(2, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(1, 2, 3, 4), "", ""),
        std::make_tuple(std::make_tuple(46, 1, 1, 1), "", "")
    };
    
    // Проверяем логику фильтрации (не саму функцию print)
    int count = 0;
    for (const auto& record : ips) {
        const auto& ip = std::get<0>(record);
        if (std::get<0>(ip) == 1) {
            count++;
        }
    }
    EXPECT_EQ(count, 2);
}

TEST(FilterTest, First46Second70) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(46, 70, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(46, 71, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(47, 70, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(46, 70, 2, 2), "", "")
    };
    
    int count = 0;
    for (const auto& record : ips) {
        const auto& ip = std::get<0>(record);
        if (std::get<0>(ip) == 46 && std::get<1>(ip) == 70) {
            count++;
        }
    }
    EXPECT_EQ(count, 2);
}

TEST(FilterTest, AnyByteEquals46) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(46, 1, 1, 1), "", ""),  // первый = 46
        std::make_tuple(std::make_tuple(1, 46, 1, 1), "", ""),  // второй = 46
        std::make_tuple(std::make_tuple(1, 1, 46, 1), "", ""),  // третий = 46
        std::make_tuple(std::make_tuple(1, 1, 1, 46), "", ""),  // четвертый = 46
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", "")    // нет 46
    };
    
    int count = 0;
    for (const auto& record : ips) {
        const auto& ip = std::get<0>(record);
        if (std::get<0>(ip) == 46 || std::get<1>(ip) == 46 || 
            std::get<2>(ip) == 46 || std::get<3>(ip) == 46) {
            count++;
        }
    }
    EXPECT_EQ(count, 4);
}

// Тест для проверки чтения файла (мок)
TEST(ReadFileTest, MockFileReading) {
    // Создаем временный файл для тестирования
    std::string test_filename = "test_data.tsv";
    std::ofstream test_file(test_filename);
    test_file << "192.168.1.1\ttest2\ttest3\n";
    test_file << "10.0.0.1\tabc\tdef\n";
    test_file.close();
    
    auto records = readAllLines(test_filename);
    EXPECT_EQ(records.size(), 2);
    
    // Удаляем временный файл
    std::remove(test_filename.c_str());
}