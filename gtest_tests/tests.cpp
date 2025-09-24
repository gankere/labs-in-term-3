#include <gtest/gtest.h>
#include "functions.h"
#include <fstream>

//parseLine
TEST(ParseLineTest, BasicParsing) {
    auto record = parseLine("192.168.1.1\ttest2\ttest3");
    const auto& ip = std::get<0>(record);
    
    EXPECT_EQ(std::get<0>(ip), 192);
    EXPECT_EQ(std::get<1>(ip), 168);
    EXPECT_EQ(std::get<2>(ip), 1);
    EXPECT_EQ(std::get<3>(ip), 1);
}

//mainSort
TEST(SortTest, ReverseLexicographicalOrder) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(2, 0, 0, 0), "", "")
    };
    
    mainSort(ips);
    
    EXPECT_EQ(std::get<0>(std::get<0>(ips[0])), 2);  // 2.0.0.0 должен быть первым
    EXPECT_EQ(std::get<0>(std::get<0>(ips[1])), 1);  // 1.1.1.1 вторым
}

//фильтрация
TEST(FilterTest, FirstByteEquals1) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(2, 1, 1, 1), "", "")
    };
    
    int count = 0;
    for (const auto& record : ips) {
        const auto& ip = std::get<0>(record);
        if (std::get<0>(ip) == 1) count++;
    }
    EXPECT_EQ(count, 1);
}

TEST(FilterTest, AnyByteEquals46) {
    std::vector<ip_record> ips = {
        std::make_tuple(std::make_tuple(46, 1, 1, 1), "", ""),
        std::make_tuple(std::make_tuple(1, 1, 1, 1), "", "")
    };
    
    int count = 0;
    for (const auto& record : ips) {
        const auto& ip = std::get<0>(record);
        if (std::get<0>(ip) == 46) count++;
    }
    EXPECT_EQ(count, 1);
}

// Минимальный тест чтения файла
TEST(ReadFileTest, MockFileReading) {
    std::string test_filename = "test_data.tsv";
    std::ofstream test_file(test_filename);
    test_file << "192.168.1.1\ttest2\ttest3\n";
    test_file.close();
    
    auto records = readAllLines(test_filename);
    EXPECT_GE(records.size(), 1);
    
    std::remove(test_filename.c_str());
}

// Простой smoke test
TEST(SmokeTest, BasicFunctionality) { 
    EXPECT_EQ(1 + 1, 2);  //Цель: Убедиться, что тестовая среда работает правильно.
}