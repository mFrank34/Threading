//
// Created by frank on 15/02/2026.
//

#include "Basic.h"

/*
 * Basic Thread Creation and Management
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

/**
 * example simple function to pass to thread
 */
void print_message(int id)
{
    std::cout << "hello from thread: " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "thread " << id << " finishing" << std::endl;
}

/**
 * function with parameters
 */
void count_down(int start, const std::string& name)
{
    for (int i = start; i >= 0; --i)
    {
        std::cout << name << ": " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

/**
 * basic thread creation and joining
 */
void basic()
{
    std::cout << "example 1: Basic Thread" << std::endl;
    std::thread t1(print_message, 1);

    // must join or detach before the thread object is destroyed
    t1.join();

    std::cout << "Main thread continues after t1 joins" << std::endl;
}

/**
 * multiple threads
 */
void multiple()
{
    std::cout << "example 2: Multiple Threads" << std::endl;
    std::vector<std::thread> threads;

    // Create 5 threads
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(print_message, i);
    }

    // Join all threads
    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "All threads completed" << std::endl;
}

void arguments()
{
    std::cout << "example 3: Thread Arguments" << std::endl;

    std::thread t1(count_down, 5, "Counter-1");
    std::thread t2(count_down, 3, "Counter-2");

    t1.join();
    t2.join();
}

/**
 * threads and lamba's
 */
void lambda()
{
    std::cout << "example 4: Lambda Threads" << std::endl;

    int value = 42;

    std::thread t1([value]()
    {
        std::cout << "Lambda thread with captured value: " << value << std::endl;
    });

    std::thread t2([]()
    {
        for (int i = 0; i < 3; ++i)
        {
            std::cout << "Lambda iteration " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    t1.join();
    t2.join();
}

/**
 * detached Thread Example
 */
void detach()
{
    std::cout << "example 5: Detached Thread" << std::endl;

    std::thread t1([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Detached thread running\n";
    });

    t1.detach(); // Thread runs independently

    // Warning: Main might exit before detached thread finishes
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
