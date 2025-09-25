#include "functions.h"

int main(){
    auto records = readAllLines(IP_FILTER_PATH); //буквально вектор с кортежами
    
    mainSort(records);
    printFullList(records);
    //Без лямда-функций 
    // printWith1(records);
    // printWith46And70(records);
    // printWith46(records);

    //С лямда-функциями
    printFiltered(records, [](const ip_address& ip) { return std::get<0>(ip) == 1; });
    printFiltered(records, [](const ip_address& ip) { return std::get<0>(ip) == 46 && std::get<1>(ip) == 70; });
    printFiltered(records, [](const ip_address& ip) { 
        return std::get<0>(ip) == 46 || std::get<1>(ip) == 46 || 
            std::get<2>(ip) == 46 || std::get<3>(ip) == 46; 
    });
    
    return 0;
}