//
// Created by frank on 15/02/2026.
//

#include "Pool.h"

#include <ostream>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

ThreadPool::ThreadPool(size_t num_threads)
    : stop_(false), active_task_(0), completed_task_(0)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back([this, i]
        {
            worker_thread(i);
        });
    }
    std::cout << "Thread pool created with " << num_threads << " workers" << std::endl;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        stop_ = true;
    }

    cv_.notify_all();

    for (auto& worker : workers_)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }

    std::cout << "Thread pool destroyed" << std::endl;
}

int ThreadPool::get_active_tasks() const
{
    return active_task_;
}

int ThreadPool::get_completed_tasks() const
{
    return completed_task_;
}

int ThreadPool::get_pending_tasks()
{
    std::unique_lock<std::mutex> lock(queue_mtx_);
    return tasks_.size();
}

void ThreadPool::worker_thread(int id)
{
    std::cout << "Worker " << id << " started" << std::endl;
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mtx_);
            cv_.wait(lock, [this]
            {
                return stop_ || !tasks_.empty();
            });

            // Exit if we're stopping and no tasks remain
            if (stop_ && tasks_.empty())
            {
                break;
            }

            // Get next task
            if (!tasks_.empty())
            {
                task = std::move(tasks_.front());
                tasks_.pop();
            }
        }
        if (task)
        {
            active_task_++;
            task();
            active_task_--;
            completed_task_++;
        }
    }
    std::cout << "Worker " << id << " completed" << std::endl;
}

void request(int request_id)
{
    std::cout << "Processing request " << request_id
        << " on thread " << std::this_thread::get_id() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(500));

    std::cout << "Completed request: " << request_id
        << " on thread " << std::this_thread::get_id() << std::endl;
}

void compute_task(int id, int value)
{
    std::cout << "Computing task " << id << ": value = " << value << std::endl;

    int result = 0;
    for (int i = 0; i < 1000000; ++i)
    {
        result += i % value;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Task: " << id << " result: " << result << std::endl;
}

void basic_usage()
{
    std::cout << "example 3: Tasks with Shared State" << std::endl;
    ThreadPool pool(4);

    std::mutex result_mtx;
    int total_sum = 0;

    // Submit tasks that update shared state
    for (int i = 1; i <= 20; ++i)
    {
        pool.enqueue([i, &total_sum, &result_mtx]
        {
            int local_sum = 0;
            for (int j = 0; j < 100; ++j)
            {
                local_sum += j;
            }

            // Safely update shared state
            {
                std::lock_guard<std::mutex> lock(result_mtx);
                total_sum += local_sum;
            }

            std::cout << "Task " << i << " contributed to sum" << std::endl;
        });
    }

    // Wait for all tasks
    while (pool.get_completed_tasks() < 20)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << std::endl << "Total sum from all tasks: " << total_sum << std::endl;
}

void dynamic_tasks()
{
    std::cout << "example 2: Dynamic Task Submission" << std::endl;
    ThreadPool pool(3);

    // Submit different types of tasks
    for (int i = 1; i <= 5; ++i)
    {
        pool.enqueue([i]
        {
            compute_task(i, 100 + i);
        });
    }

    // Add more tasks while others are running
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    for (int i = 6; i <= 8; ++i)
    {
        pool.enqueue([i]
        {
            std::cout << "Late task " << i << " starting\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "Late task " << i << " done\n";
        });
    }

    // Wait for completion
    while (pool.get_completed_tasks() < 8)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void shared_state()
{
    std::cout << "example 3: Tasks with Shared State" << std::endl;

    ThreadPool pool(4);

    std::mutex result_mtx;
    int total_sum = 0;

    // Submit tasks that update shared state
    for (int i = 1; i <= 20; ++i)
    {
        pool.enqueue([i, &total_sum, &result_mtx]
        {
            int local_sum = 0;
            for (int j = 0; j < 100; ++j)
            {
                local_sum += j;
            }

            // Safely update shared state
            {
                std::lock_guard<std::mutex> lock(result_mtx);
                total_sum += local_sum;
            }

            std::cout << "Task " << i << " contributed to sum" << std::endl;
        });
    }

    // Wait for all tasks
    while (pool.get_completed_tasks() < 20)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << std::endl << "Total sum from all tasks: " << total_sum << std::endl;
}
