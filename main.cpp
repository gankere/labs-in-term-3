#include "functions.h"

int main(){
    auto records = readAllLines(IP_FILTER_PATH); //буквально вектор с кортежами
    
    mainSort(records);
    printFullList(records);
    printWith1(records);
    printWith46And70(records);
    printWith46(records);

    return 0;
}