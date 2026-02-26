#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "../types/thread_guard.h"

TEST(Thread_guard_tests, Constructor_moves_thread)
{
    std::thread t([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    
    thread_guard guard(std::move(t));

    ASSERT_FALSE(t.joinable());
}

TEST(Thread_guard_tests, Constructor_correct_for_non_joinable)
{
    std::thread t;
    
    {
        thread_guard guard(std::move(t));
    }

    SUCCEED();
}