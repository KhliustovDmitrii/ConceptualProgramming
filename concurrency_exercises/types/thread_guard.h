#ifndef _THREAD_GUARD_H_
#define _THREAD_GUARD_H_
#include <thread>

/*
RAII wrapper for thread object
Ensures the thread is correctly joined in all cases
*/
class thread_guard
{
private:
    std::thread thr;

public:
    explicit thread_guard(std::thread t) : thr(std::move(t)) {};

    thread_guard(thread_guard&& other)
    {
        thr = std::move(other.thr);
    };

    thread_guard& operator=(thread_guard&& other)
    {
        if(this != &other)
        {
            thr = std::move(other.thr);
        }

        return *this;
    };

    ~thread_guard()
    {
        if(thr.joinable())
        {
            thr.join();
        }
    };

    thread_guard(thread_guard const &) = delete;
    thread_guard& operator=(thread_guard const &) = delete;
};

#endif