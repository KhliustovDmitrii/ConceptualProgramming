#include <gtest/gtest.h>

#include "arithmetics_tests.cpp"
#include "typelist_tests.cpp"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}