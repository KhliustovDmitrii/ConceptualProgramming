#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <vector>

#include "../types/threadsafe_queue.h"

TEST(Threadsafe_queue_tests, Basic_functionality_test)
{
    threadsafe_queue<int> q;

    ASSERT_TRUE(q.empty());
    ASSERT_EQ(q.try_pop(), nullptr);

    q.push(1);
    q.push(2);
    q.push(3);

    ASSERT_FALSE(q.empty());

    int res;
    bool success = q.try_pop(res);
    ASSERT_TRUE(success);
    ASSERT_EQ(res, 1);
}

TEST(Threadsafe_queue_tests, Producer_consumer_test)
{
    threadsafe_queue<int> q;

    int num_elements = 10;
    auto data_preparation_func = [&q, num_elements](){
        for(int i = 0; i<num_elements; i++)
        {
            q.push(i);
        }
    };

    std::vector<int> result;
    result.reserve(num_elements);

    auto data_processing_func = [&q, &result, num_elements](){
        while(result.size() < num_elements)
        {
            auto elem = q.wait_and_pop();
            result.push_back(*elem);
        }
    };

    std::thread producer(data_preparation_func);
    std::thread consumer(data_processing_func);

    producer.join();
    consumer.join();

    std::vector<int> expected_result;
    for(int i = 0; i<num_elements; i++)
        expected_result.push_back(i);

    ASSERT_EQ(result, expected_result);
}