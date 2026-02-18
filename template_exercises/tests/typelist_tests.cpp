#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <type_traits>

#include "../types/typelist.h"
#include "../types/arithmetics.h"

TEST(Basic_typelist_tests, Length_calculated_correctly)
{
    using namespace typelist;
    ASSERT_EQ(Length<NullType>::value, 0);

    int l = Length<Typelist<int, NullType>>::value;
    ASSERT_EQ(l, 1);
}

TEST(Basic_typelist_tests, Linearized_template_creator_correct)
{
    using namespace typelist;
    
    using regular = Typelist<int, Typelist<double, NullType>>;
    using linearized = LinearizedTemplateCreator<int, double>::Result;

    static_assert(std::is_same_v<regular, linearized>);
}

TEST(Basic_typelist_tests, Append_working_correctly)
{
    using namespace typelist;

    using original = LinearizedTemplateCreator<int, double>::Result;
    using result_expected = LinearizedTemplateCreator<int, double, float>::Result;
    using result = Append<original, float>::Result;

    static_assert(std::is_same_v<result, result_expected>);
    static_assert(std::is_same_v<Append<NullType, int>::Result, Typelist<int, NullType>>);
}

TEST(Basic_typelist_tests, Access_working_correctly)
{
    using namespace typelist;

    using list = LinearizedTemplateCreator<int, int, double>::Result;
    static_assert(std::is_same_v<Access<list, 0>::Result, int>);
    static_assert(std::is_same_v<Access<list, 1>::Result, int>);
    static_assert(std::is_same_v<Access<list, 2>::Result, double>);
}

TEST(Basic_typelist_tests, DeleteFirst_working_correctly)
{
    using namespace typelist;

    using list = LinearizedTemplateCreator<int, double, int, double>::Result;
    using result_expected = LinearizedTemplateCreator<int, int, double>::Result;
    using result = DeleteFirst<double, list>::Result;
    static_assert(std::is_same_v<result, result_expected>);
}

TEST(Typelist_algorithms_tests, Reverse_working_correctly)
{
    using namespace typelist;

    using original = LinearizedTemplateCreator<int, double, float>::Result;
    using result_expected = LinearizedTemplateCreator<float, double, int>::Result;

    using result = Reverse<original>::Result;

    static_assert(std::is_same_v<result, result_expected>);
    static_assert(std::is_same_v<NullType, Reverse<NullType>::Result>);
}

TEST(Typelist_algorithms_tests, Map_working_correctly)
{
    using namespace typelist;

    using original = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<0>,
    arithmetic_templates::IntToType<1>,
    arithmetic_templates::IntToType<2>
    >::Result;

    using result_expected = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<1>,
    arithmetic_templates::IntToType<2>,
    arithmetic_templates::IntToType<3>
    >::Result;

    using result = Map<original, arithmetic_templates::NextNat>::Result;

    static_assert(std::is_same_v<result, result_expected>);
}

TEST(Typelist_algorithms_tests, Reduce_working_correctly)
{
    using namespace typelist;

    using list = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<1>,
    arithmetic_templates::IntToType<2>,
    arithmetic_templates::IntToType<3>
    >::Result;

    using result_expected = arithmetic_templates::IntToType<6>;

    using result = Reduce<list, arithmetic_templates::Sum, arithmetic_templates::IntToType<0>>::Result;

    static_assert(std::is_same_v<result, result_expected>);
}

TEST(Typelist_algorithms_tests, Max_working_correctly)
{
    using namespace typelist;

    using list = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<1>,
    arithmetic_templates::IntToType<3>,
    arithmetic_templates::IntToType<2>
    >::Result;

    using result_expected = arithmetic_templates::IntToType<3>;
    using result = Max<list>::Result;

    static_assert(std::is_same_v<result, result_expected>);
}

TEST(Typelist_algorithms_tests, Sort_working_correctly)
{
    using namespace typelist;

    using list = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<1>,
    arithmetic_templates::IntToType<3>,
    arithmetic_templates::IntToType<2>,
    arithmetic_templates::IntToType<3>,
    arithmetic_templates::IntToType<5>,
    >::Result;

    using result_expected = LinearizedTemplateCreator<
    arithmetic_templates::IntToType<5>,
    arithmetic_templates::IntToType<3>,
    arithmetic_templates::IntToType<3>,
    arithmetic_templates::IntToType<2>,
    arithmetic_templates::IntToType<1>
    >::Result;

    using result = Sort<list>::Result;

    static_assert(std::is_same_v<result, result_expected>);
}