//
// Created by frank on 15/02/2026.
//

#include "Mutexes.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

void race_condition()
{
    std::cout << "example 1: Race Condition (UNSAFE)" << std::endl;
    int counter = 0;

    auto increment = [&counter]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            counter++; // NOT thread-safe!
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);
    std::thread t3(increment);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final counter (should be 3000): " << counter << std::endl;
    std::cout << "This is likely wrong due to race condition!" << std::endl;
}

void race_mutex()
{
    std::cout << "example 2: With Mutex (SAFE)" << std::endl;

    int counter = 0;
    std::mutex counter_mtx;

    auto increment = [&counter, &counter_mtx]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            counter_mtx.lock();
            counter++;
            counter_mtx.unlock();
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);
    std::thread t3(increment);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final counter: " << counter << std::endl;
}

void lock_guard()
{
    std::cout << "example 3: Lock Guard (Recommended)" << std::endl;

    int counter = 0;
    std::mutex counter_mtx;

    auto increment = [&counter, &counter_mtx]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            std::lock_guard<std::mutex> lock(counter_mtx);
            counter++;
            // Automatically unlocks when lock goes out of scope
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);
    std::thread t3(increment);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final counter: " << counter << std::endl;
}

void unique_lock()
{
    std::cout << "example 4: Unique Lock" << std::endl;

    std::mutex print_mtx;

    auto print_thread_id = [&print_mtx](int id)
    {
        std::unique_lock<std::mutex> lock(print_mtx);
        std::cout << "Thread " << id << " acquired lock" << std::endl;

        // Can manually unlock if needed
        lock.unlock();

        // Do some work without lock
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Re-lock if needed
        lock.lock();
        std::cout << "Thread " << id << " finishing" << std::endl;
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i)
    {
        threads.emplace_back(print_thread_id, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

ThreadSafeCounter::ThreadSafeCounter() : value_(0)
{
}

void ThreadSafeCounter::increment()
{
    std::lock_guard<std::mutex> lock(mtx_);
    ++value_;
}

void ThreadSafeCounter::decrement()
{
    std::lock_guard<std::mutex> lock(mtx_);
    --value_;
}

int ThreadSafeCounter::get() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return value_;
}

void thread_safe_class()
{
    std::cout << "example 5: Thread Safe Class" << std::endl;
    ThreadSafeCounter counter;

    auto increment_many = [&counter]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            counter.increment();
        }
    };

    auto decrement_many = [&counter]()
    {
        for (int i = 0; i < 500; ++i)
        {
            counter.decrement();
        }
    };

    std::thread t1(increment_many);
    std::thread t2(increment_many);
    std::thread t3(decrement_many);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final counter value: " << counter.get() << std::endl;
}

void try_lock()
{
    std::cout << "example 6: Try Lock" << std::endl;
    std::mutex mtx;

    auto try_access = [&mtx](int id)
    {
        if (mtx.try_lock())
        {
            std::cout << "Thread " << id << " acquired lock\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mtx.unlock();
        }
        else
        {
            std::cout << "Thread " << id << " couldn't acquire lock\n";
        }
    };

    std::thread t1(try_access, 1);
    std::thread t2(try_access, 2);

    t1.join();
    t2.join();
}
