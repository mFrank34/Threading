//
// Created by frank on 15/02/2026.
//

#include <iostream>
#include <thread>
#include "basic/Basic.h"
#include "mutexes/Mutexes.h"

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
    std::cout << std::endl;

    std::cout << "C++ Thread Synchronization with Mutexes" << std::endl << std::endl;

    race_condition();
    std::cout << std::endl;

    race_mutex();
    std::cout << std::endl;

    lock_guard();
    std::cout << std::endl;

    unique_lock();
    std::cout << std::endl;

    thread_safe_class();
    std::cout << std::endl;

    try_lock();
    std::cout << std::endl;


    return 0;
}
