#include <gtest/gtest.h>
#include <thread>

#include "../types/threadsafe_stack.h"

TEST(Threadsafe_stack_tests, Basic_functionality)
{
    threadsafe_stack<int> stack;

    stack.push(5);
    stack.push(10);

    int v1;
    stack.pop(v1);

    ASSERT_EQ(v1, 10);
    auto v2 = stack.pop();

    ASSERT_EQ(*v2, 5);

    ASSERT_THROW(stack.pop(), empty_stack);
}