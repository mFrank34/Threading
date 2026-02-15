//
// Created by frank on 15/02/2026.
//

#include <iostream>
#include <thread>
#include "basic/Basic.h"

int main()
{
    std::cout << "C++ Threading Basics" << std::endl;
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads" << std::endl;

    basic();
    std::cout << std::endl;

    multiple();
    std::cout << std::endl;

    arguments();
    std::cout << std::endl;

    lambda();
    std::cout << std::endl;
    detach();

    return 0;
}