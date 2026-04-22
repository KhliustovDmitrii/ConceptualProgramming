#include <gtest/gtest.h>

#include "../algorithms/quick_sort.h"

TEST(Quick_sort_tests, Basic_functionality_test)
{
    std::list<int> input = {5, 3, 9, 1, 0, 8};
    std::list<int> result_expected = {0, 1, 3, 5 ,8, 9};
    
    auto result = quick_sort(input);

    ASSERT_EQ(result, result_expected);
}