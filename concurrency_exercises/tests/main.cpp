#include <gtest/gtest.h>

#include "thread_guard_tests.cpp"
#include "threadsafe_stack_tests.cpp"
#include "hierarchical_mutex_tests.cpp"
#include "threadsafe_queue_tests.cpp"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}