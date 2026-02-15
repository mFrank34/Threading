//
// Created by frank on 15/02/2026.
//

#ifndef CONDITION_H
#define CONDITION_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>

void wait_notify();

void producer_consumer();

template <typename T>
class BoundedBuffer
{
private:
    std::queue<T> buffer_;
    size_t capacity_;
    std::mutex mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    BoundedBuffer(size_t capacity) : capacity_(capacity)
    {
    }

    void push(T item)
    {
        std::unique_lock<std::mutex> lock(mtx_);

        // Wait until buffer is not full
        not_full_.wait(lock, [this] { return buffer_.size() < capacity_; });

        buffer_.push(item);
        std::cout << "Pushed item (buffer size: " << buffer_.size() << ")\n";

        not_empty_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);

        // Wait until buffer is not empty
        not_empty_.wait(lock, [this] { return !buffer_.empty(); });

        T item = buffer_.front();
        buffer_.pop();
        std::cout << "Popped item (buffer size: " << buffer_.size() << ")\n";

        not_full_.notify_one();
        return item;
    }
};

void bounded_buffer();

void barrier();

#endif // CONDITION_H
