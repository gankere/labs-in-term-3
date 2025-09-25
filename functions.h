#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>   
#include <fstream>    
#include <string>     
#include <tuple>      
#include <vector>     
#include <algorithm>  
#include <sstream>    
#include <array>
#include <functional>

using ip_address = std::tuple<int, int, int, int>;
using ip_record = std::tuple<ip_address, std::string, std::string>;

ip_record parseLine(const std::string& line);
std::vector<ip_record> readAllLines (const std::string& filename);
void printRecord(const ip_record& record);

void mainSort(std::vector<ip_record>& ip_pool);
void printFullList(const std::vector<ip_record>& ip_pool);
void printFiltered(const std::vector<ip_record>& ip_pool, std::function<bool(const ip_address&)> filter);
// void printWith1(const std::vector<ip_record>& ip_pool);
// void printWith46And70(const std::vector<ip_record>& ip_pool);
// void printWith46(const std::vector<ip_record>& ip_pool);

#endif