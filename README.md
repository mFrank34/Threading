# C++ Threading Tutorial

A comprehensive guide to learning multithreading in C++, from basics to practical applications.

## Prerequisites

- C++11 or later (examples use C++17)
- A compiler that supports the `<thread>` library (g++, clang++)
- Basic understanding of C++ (classes, lambdas, RAII)

## Compilation

All examples can be compiled with:

```bash
g++ -std=c++17 -pthread filename.cpp -o output_name
```

## Learning Path

### 1. Basic Threads (`01_basic_threads.cpp`)

**Start here!** Learn the fundamentals:

- Creating and joining threads
- Passing arguments to threads
- Using lambda functions with threads
- Understanding `join()` vs `detach()`
- Managing multiple threads

**Key Concepts:**

- `std::thread` - the basic thread object
- `join()` - wait for a thread to complete
- `detach()` - let thread run independently
- `hardware_concurrency()` - detect available CPU cores

### 2. Mutexes and Synchronization (`02_mutexes.cpp`)

Learn how to protect shared data:

- Understanding race conditions
- Using `std::mutex` for thread safety
- `lock_guard` for automatic lock management (RAII)
- `unique_lock` for more flexibility
- Building thread-safe classes

**Key Concepts:**

- **Race condition** - multiple threads accessing shared data unsafely
- `std::mutex` - mutual exclusion lock
- `std::lock_guard` - RAII wrapper (recommended for most cases)
- `std::unique_lock` - more flexible lock management
- `try_lock()` - non-blocking lock attempt

### 3. Condition Variables (`03_condition_variables.cpp`)

Learn thread coordination and communication:

- Wait and notify mechanisms
- Producer-consumer pattern
- Bounded buffer implementation
- Barrier synchronization

**Key Concepts:**

- `std::condition_variable` - thread signaling mechanism
- `wait()` - block until notified and condition is true
- `notify_one()` - wake up one waiting thread
- `notify_all()` - wake up all waiting threads
- Spurious wakeups and why we need predicates

### 4. Thread Pool (`04_thread_pool.cpp`)

Put it all together with a practical example:

- Building a reusable thread pool
- Task queue management
- Worker thread lifecycle
- Real-world patterns

**Key Concepts:**

- Thread pool pattern for reusing threads
- Task queue with condition variables
- Combining mutexes and condition variables
- Clean shutdown procedures

## Common Patterns

### Pattern 1: RAII Lock Management

```cpp
// Always prefer lock_guard or unique_lock over manual lock/unlock
{
    std::lock_guard<std::mutex> lock(mtx);
    // critical section
} // automatically unlocks
```

### Pattern 2: Condition Variable Wait

```cpp
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [&]{ return condition_is_true; });
// Only proceeds when condition is true
```

### Pattern 3: Thread-Safe Queue

```cpp
std::queue<T> queue;
std::mutex mtx;
std::condition_variable cv;

// Push
{
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(item);
}
cv.notify_one();

// Pop
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [&]{ return !queue.empty(); });
T item = queue.front();
queue.pop();
```

## Best Practices

### ✅ DO:

1. **Always join or detach threads** before they go out of scope
2. **Use RAII lock guards** (`lock_guard`, `unique_lock`) instead of manual lock/unlock
3. **Keep critical sections small** - hold locks for minimal time
4. **Use condition variables** for thread coordination (don't busy-wait!)
5. **Design thread-safe interfaces** - encapsulate synchronization in classes
6. **Test with thread sanitizer** (`-fsanitize=thread`)

### ❌ DON'T:

1. **Don't access shared data without synchronization**
2. **Don't hold multiple locks** unless you really need to (deadlock risk)
3. **Don't use `detach()` unless you know what you're doing**
4. **Don't forget to unlock** (use RAII!)
5. **Don't busy-wait** with loops - use condition variables
6. **Don't assume thread scheduling order**

## Common Pitfalls

### Race Condition

```cpp
// BAD - race condition
int counter = 0;
thread1: counter++;  // Read-Modify-Write not atomic!
thread2: counter++;

// GOOD - protected with mutex
std::mutex mtx;
{
    std::lock_guard<std::mutex> lock(mtx);
    counter++;
}
```

### Deadlock

```cpp
// BAD - can deadlock
Thread1: lock(mtx1); lock(mtx2);
Thread2: lock(mtx2); lock(mtx1);

// GOOD - always lock in same order
Thread1: lock(mtx1); lock(mtx2);
Thread2: lock(mtx1); lock(mtx2);

// BETTER - use std::scoped_lock (C++17)
std::scoped_lock lock(mtx1, mtx2);  // Deadlock-free
```

### Forgotten Join

```cpp
// BAD - undefined behavior!
void bad_function() {
    std::thread t(worker);
    // t destroyed without join/detach - program will terminate!
}

// GOOD
void good_function() {
    std::thread t(worker);
    t.join();  // or t.detach() if appropriate
}
```

## Debugging Tips

1. **Compile with warnings:**
   ```bash
   g++ -std=c++17 -pthread -Wall -Wextra file.cpp
   ```

2. **Use thread sanitizer:**
   ```bash
   g++ -std=c++17 -pthread -fsanitize=thread file.cpp
   ./a.out
   ```

3. **Add debug output:**
   ```cpp
   std::cout << "Thread " << std::this_thread::get_id() << ": message\n";
   ```

4. **Use a debugger that supports threads** (gdb, lldb)

## Advanced Topics (for later)

Once you're comfortable with the basics, explore:

- `std::atomic` for lock-free programming
- `std::future` and `std::async` for task-based parallelism
- `std::jthread` (C++20) - auto-joining threads
- `std::latch` and `std::barrier` (C++20)
- Thread-local storage (`thread_local`)
- Memory ordering and acquire-release semantics
- Lock-free data structures

## Resources

- [C++ Reference - Thread Support](https://en.cppreference.com/w/cpp/thread)
- "C++ Concurrency in Action" by Anthony Williams
- CppCon talks on threading and concurrency

## Practice Exercises

1. **Beginner:** Modify `01_basic_threads.cpp` to calculate factorials in parallel
2. **Intermediate:** Extend `02_mutexes.cpp` to implement a thread-safe bank account
3. **Advanced:** Create a parallel merge sort using the thread pool from `04_thread_pool.cpp`

## Quick Reference

| Concept                   | Use When               | Header                 |
|---------------------------|------------------------|------------------------|
| `std::thread`             | Creating threads       | `<thread>`             |
| `std::mutex`              | Protecting shared data | `<mutex>`              |
| `std::lock_guard`         | Auto lock/unlock       | `<mutex>`              |
| `std::unique_lock`        | Need manual control    | `<mutex>`              |
| `std::condition_variable` | Thread coordination    | `<condition_variable>` |
| `std::atomic`             | Lock-free operations   | `<atomic>`             |
