#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_

#include <list>
#include <future>

template <typename T>
std::list<T> quick_sort(std::list<T> input)
{
    if(input.empty())
        return input;

    std::list<T> result;

    result.splice(result.begin(), input, input.begin());
    T const & pivot = *result.begin();

    auto divide_point = std::partition(input.begin(), input.end(),
    [&](T const& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.begin(), input, input.begin(), divide_point);

    auto lower_future = std::async(quick_sort<T>, std::move(lower_part));
    auto higher_future = std::async(quick_sort<T>, std::move(input));

    auto new_higher = higher_future.get();
    auto new_lower = lower_future.get();

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);

    return result;
}

#endif