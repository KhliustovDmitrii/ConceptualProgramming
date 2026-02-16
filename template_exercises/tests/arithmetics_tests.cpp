#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../types/arithmetics.h"

TEST(Basic_arithmetic_test, Sum_up_to_calculated_correctly)
{
    using namespace arithmetic_templates;
    int value = SumUpTo<IntToType<0>>::Result::value;
    ASSERT_EQ(value, 0);

    value = SumUpTo<IntToType<10>>::Result::value;
    ASSERT_EQ(value, 55);
}

TEST(Basic_arithmetic_test, Factorial_calculated_correctly)
{
    using namespace arithmetic_templates;
    int value = Factorial<IntToType<0>>::Result::value;
    ASSERT_EQ(value, 1);

    value = Factorial<IntToType<5>>::Result::value;
    ASSERT_EQ(value, 120);
}