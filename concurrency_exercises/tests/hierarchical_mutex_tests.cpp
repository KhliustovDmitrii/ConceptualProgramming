#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "../types/hierarchical_mutex.h"

TEST(Hierarchical_mutex_tests, Basic_logic_test)
{
    hierarchical_mutex high(1000);
    hierarchical_mutex low(500);

    high.lock();
    low.lock();

    bool try_result = true;
    std::thread other([&try_result, &high](){
        try_result = high.try_lock();
    });

    other.join();
    ASSERT_FALSE(try_result);

    low.unlock();
    high.unlock();

    low.lock();
    ASSERT_THROW(high.lock(), std::runtime_error);
}