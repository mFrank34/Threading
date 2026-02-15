//
// Created by frank on 15/02/2026.
//

#ifndef POOL_H
#define POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    template <typename F>
    void enqueue(F&& task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mtx_);
            tasks_.emplace(std::forward<F>(task));
        }
        cv_.notify_one();
    }

    int get_active_tasks() const;
    int get_completed_tasks() const;
    int get_pending_tasks();

private:
    void worker_thread(int id);

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool stop_;

    std::atomic<int> active_task_;
    std::atomic<int> completed_task_;
};

void request(int request_id);
void compute_task(int id, int value);

void basic_usage();
void dynamic_tasks();
void shared_state();

#endif // POOL_H
