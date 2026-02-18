#ifndef _UTILS_H_
#define _UTILS_H_
namespace utils
{
template <bool Pred, typename TrueALt, typename FalseAlt>
struct Conditional;

template <typename TrueAlt, typename FalseAlt>
struct Conditional<true, TrueAlt, FalseAlt>
{
    using Result = TrueAlt;
};

template <typename TrueAlt, typename FalseAlt>
struct Conditional<false, TrueAlt, FalseAlt>
{
    using Result = FalseAlt;
};
}; // utils
#endif