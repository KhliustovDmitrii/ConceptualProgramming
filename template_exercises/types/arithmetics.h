#ifndef _ARITHMETICS_H_
#define _ARITHMETICS_H_

namespace arithmetic_templates
{
// basic arithmetic notions
template <int v>
struct IntToType
{
    enum {value = v};
};

template <typename IT, typename IS>
struct Sum
{
    using Result = IntToType<IT::value + IS::value>;
};

template <typename IT, typename IS>
struct Mul
{
    using Result = IntToType<IT::value*IS::value>;
};

// define function of form 
// f(0) = a
// f(x) = h(x, f(l(x)))
template <template <typename, typename> typename h, template <typename> typename l, typename IT, typename BV>
struct Appl
{
    using Result = typename h<IT, typename Appl<h, l, typename l<IT>::Result, BV>::Result>::Result;
};

template<template <typename, typename> typename h, template <typename> typename l, typename BV>
struct Appl<h, l, IntToType<0>, BV>
{
    using Result = BV;
};


// concrete arithmetic functions

template <typename Arg>
struct ID
{
    using Result = Arg;
};

template <typename IT>
struct NextNat
{
    using Result = IntToType<IT::value + 1>;
};

template <typename IT>
struct PrevNat
{
    using Result = IntToType<IT::value - 1>;
};

template <>
struct PrevNat<IntToType<0>>
{
    using Result = IntToType<0>;
};

template <typename IT>
struct Factorial
{
    using Result = typename Appl<Mul, PrevNat, IT, IntToType<1>>::Result;
};

template <typename IT>
struct SumUpTo
{
    using Result = typename Appl<Sum, PrevNat, IT, IntToType<0>>::Result;
};
}; // arithmetic_templates
#endif