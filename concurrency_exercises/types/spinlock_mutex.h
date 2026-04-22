#ifndef _SPINLOCK_MUTEX_H_
#define _SPINLOCK_MUTEX_H_
#include <atomic>

class spinlock_mutex
{
public:
    spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {};

    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

    bool try_lock()
    {
        return !flag.test_and_set(std::memory_order_acquire);
    }

    spinlock_mutex(const spinlock_mutex&) = delete;
    spinlock_mutex& operator=(const spinlock_mutex&) = delete;
    spinlock_mutex(spinlock_mutex&&) = delete;
    spinlock_mutex& operator=(spinlock_mutex&&) = delete;

private:
    std::atomic_flag flag;
};

#endif