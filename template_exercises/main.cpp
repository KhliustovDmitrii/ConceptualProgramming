#include <iostream>
#include "types/compile_time_asserts.h"

int main()
{
    STATIC_CHECK(2>0, int_comparison_error);
    return 0;
}