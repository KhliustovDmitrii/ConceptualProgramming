#ifndef _HIERARCHICAL_MUTEX_H_
#define _HIERARCHICAL_MUTEX_H_

#include <mutex>
#include <climits>
#include <exception>

class hierarchical_mutex
{
public:

explicit hierarchical_mutex(unsigned long h_val) : 
hierarchy_value(h_val),
previous_hierarchy_value(0) {};

void lock();
void unlock();
bool try_lock();

private:
    void check_hierarchy_violation();
    void update_hierarchy_value();

    std::mutex internal_mutex;

    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;

    static thread_local unsigned long this_thread_hierarchy_value;
};

thread_local unsigned long
hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

void hierarchical_mutex::lock()
{
    check_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
}

void hierarchical_mutex::unlock()
{
    internal_mutex.unlock();
    this_thread_hierarchy_value = previous_hierarchy_value;
}

bool hierarchical_mutex::try_lock()
{
    check_hierarchy_violation();

    if(!internal_mutex.try_lock())
        return false;

    update_hierarchy_value();
    return true;
}

void hierarchical_mutex::check_hierarchy_violation()
{
    if(this_thread_hierarchy_value <= hierarchy_value)
        throw std::runtime_error("mutex hierarchy violated");
}

void hierarchical_mutex::update_hierarchy_value()
{
    previous_hierarchy_value = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchy_value;
}
#endif