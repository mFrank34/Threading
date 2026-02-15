//
// Created by frank on 15/02/2026.
//

#ifndef MUTEXES_H
#define MUTEXES_H
#include <mutex>

/**
 * threaded race conditions understanding
 */
void race_condition();

/**
 * fixed version of race conditions with thread safe
 */
void race_mutex();

/**
 * Understanding of lock guard in threads and how they work
 */
void lock_guard();

/**
 * Understanding of unique lock
 */
void unique_lock();


/**
 * Understanding on making thread safe classes
 */
class ThreadSafeCounter
{
private:
    mutable std::mutex mtx_;
    int value_;

public:
    ThreadSafeCounter();

    void increment();

    void decrement();

    int get() const;
};

/**
 * Creating the thread safe class and example of using it
 */
void thread_safe_class();

/**
 * Understanding of try lock system
 */
void try_lock();

#endif // MUTEXES_H
