#include <gtest/gtest.h>
#include <optional>

#include "../types/threadsafe_lookup_table.h"

TEST(Threadsafe_lookup_table_tests, Basic_test)
{
    threadsafe_lookup_table<int, int> table;

    auto empty_result = table.value_for(0);
    ASSERT_EQ(empty_result, std::nullopt);

    table.add_update_mapping(0, 3);
    table.add_update_mapping(5, 10);

    auto result = table.value_for(0);
    ASSERT_EQ(result.value(), 3);

    table.add_update_mapping(0, 4);
    result = table.value_for(0);
    ASSERT_EQ(result.value(), 4);

    table.remove_mapping(5);
    empty_result = table.value_for(5);
    ASSERT_EQ(empty_result, std::nullopt);
}