//
// Created by frank on 15/02/2026.
//

#include "Condition.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>

void wait_notify()
{
    std::cout << "example 1: Simple Wait / Notify" << std::endl;

    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    auto worker = [&]()
    {
        std::cout << "Worker: Waiting for signal...\n";

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return ready; }); // Wait until ready is true

        std::cout << "Worker: Received signal, starting work!\n";
    };

    std::thread t(worker);

    // Main thread prepares data
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Main: Preparing data...\n";

    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }

    std::cout << "Main: Notifying worker\n";
    cv.notify_one();

    t.join();
}

void producer_consumer()
{
    std::cout << "example 2: Producer-Consumer" << std::endl;

    std::queue<int> queue;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

    // Producer thread
    auto producer = [&]()
    {
        for (int i = 1; i <= 5; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            {
                std::lock_guard<std::mutex> lock(mtx);
                queue.push(i);
                std::cout << "Produced: " << i << std::endl;
            }

            cv.notify_one();
        }

        // Signal we're done
        {
            std::lock_guard<std::mutex> lock(mtx);
            finished = true;
        }
        cv.notify_all();
    };

    // Consumer thread
    auto consumer = [&](int id)
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx);

            // Wait for data or finish signal
            cv.wait(lock, [&] { return !queue.empty() || finished; });

            // Process all available items
            while (!queue.empty())
            {
                int value = queue.front();
                queue.pop();
                lock.unlock();

                std::cout << "Consumer " << id << " consumed: " << value << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                lock.lock();
            }

            if (finished && queue.empty())
            {
                break;
            }
        }
        std::cout << "Consumer " << id << " finished\n";
    };

    std::thread prod(producer);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);

    prod.join();
    cons1.join();
    cons2.join();
}

void bounded_buffer()
{
    std::cout << "example 3: Bounded Buffer" << std::endl;

    BoundedBuffer<int> buffer(3); // Capacity of 3

    auto producer = [&](int id)
    {
        for (int i = 0; i < 5; ++i)
        {
            int value = id * 10 + i;
            buffer.push(value);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    auto consumer = [&](int id)
    {
        for (int i = 0; i < 5; ++i)
        {
            int value = buffer.pop();
            std::cout << "Consumer " << id << " got: " << value << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    };

    std::thread p1(producer, 1);
    std::thread p2(producer, 2);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();
}

void barrier()
{
    std::cout << "example 4: Barrier Synchronization" << std::endl;

    const int NUM_THREADS = 3;
    int counter = 0;
    std::mutex mtx;
    std::condition_variable cv;

    auto worker = [&](int id)
    {
        std::cout << "Thread " << id << " working on phase 1...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));

        // Barrier: wait for all threads
        {
            std::unique_lock<std::mutex> lock(mtx);
            counter++;
            std::cout << "Thread " << id << " reached barrier (" << counter << "/" << NUM_THREADS << ")\n";

            if (counter < NUM_THREADS)
            {
                cv.wait(lock, [&] { return counter >= NUM_THREADS; });
            }
            else
            {
                cv.notify_all();
            }
        }

        std::cout << "Thread " << id << " proceeding to phase 2\n";
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }
}
