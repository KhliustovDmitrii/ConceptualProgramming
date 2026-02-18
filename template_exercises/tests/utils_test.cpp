#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <type_traits>

#include "../types/utils.h"
#include "../types/arithmetics.h"

TEST(Basic_utils_tests, Conditional_working_correctly)
{
    using namespace utils;

    using result = Conditional<
    (arithmetic_templates::IntToType<1>::value > arithmetic_templates::IntToType<0>::value),
    arithmetic_templates::IntToType<10>,
    arithmetic_templates::IntToType<20>>::Result;

    static_assert(std::is_same_v<result, arithmetic_templates::IntToType<10>>);
}