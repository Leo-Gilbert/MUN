#include "List.h"
#include <iostream>

int main(){
    List<double> myL;
    myL.push_back(2.2);
    myL.push_back(1.1);
    myL.push_back(3.3);
    const List<double> dog(myL);
    std::cout<< myL.size();
}