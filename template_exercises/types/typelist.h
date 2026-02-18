#ifndef _TYPELIST_H_
#define _TYPELIST_H_

#include "utils.h"

namespace typelist
{
// singly linked list
// L = <T1, <T2, ... NullType>>

class NullType {};

template <typename T, typename U>
struct Typelist
{
    using Head = T;
    using Tail = U;
};

// ------ Basic operations

// calculate the length
template <typename TList>
struct Length;

template <> struct 
Length<NullType>
{
    enum {value = 0};
};

template <typename T, typename U> struct 
Length<Typelist<T, U>>
{
    enum {value = 1 + Length<U>::value};
};

// linearized syntax for creating typelist
template <typename... Types> struct
LinearizedTemplateCreator;

template <typename T, typename... Rest> struct
LinearizedTemplateCreator<T, Rest...>
{
    using Result = Typelist<T, typename LinearizedTemplateCreator<Rest...>::Result>;
};

template <> struct
LinearizedTemplateCreator<>
{
    using Result = NullType;
};

// append
template <typename TList, typename T>
struct Append;

template <>
struct Append<NullType, NullType>
{
    using Result = NullType;
};

template <typename T>
struct Append<NullType, T>
{
    using Result = Typelist<T, NullType>;
};

template <typename H, typename T>
struct Append<NullType, Typelist<H, T>>
{
    using Result = Typelist<H, T>;
};

template <typename H, typename Tail, typename T>
struct Append<Typelist<H, Tail>, T>
{
    using Result = Typelist<H, typename Append<Tail, T>::Result>;
};

// access
template <typename TList, unsigned int index>
struct Access;

template <typename H, typename T>
struct Access<Typelist<H, T>, 0>
{
    using Result = H;
};

template <typename H, typename T, unsigned int index>
struct Access<Typelist<H, T>, index>
{
    using Result = Access<T, index-1>::Result;
};

// delete first occurence of element
template <typename E, typename TList>
struct DeleteFirst;

template <typename E>
struct DeleteFirst<E, NullType>
{
    using Result = NullType;
};

template <typename E, typename T>
struct DeleteFirst<E, Typelist<E, T>>
{
    using Result = T;
};

template <typename E, typename H, typename T>
struct DeleteFirst<E, Typelist<H, T>>
{
    using Result = Typelist<H, DeleteFirst<E, T>::Result>;
};

// ------ General algorithms

// list reversion
template <typename TList>
struct Reverse;

template <>
struct Reverse<NullType>
{
    using Result = NullType;
};

template <typename H, typename T>
struct Reverse<Typelist<H, T>>
{
    using Result = typename Append<typename Reverse<T>::Result, H>::Result;
};

// map
template <typename TList, template <typename> typename Function>
struct Map;

template <template <typename> typename Function>
struct Map<NullType, Function>
{
    using Result = NullType;
};

template <typename H, typename T, template <typename> typename Function>
struct Map<Typelist<H, T>, Function>
{
    using Result = Typelist<typename Function<H>::Result, Map<T, Function>::Result>;
};

// reduce
template 
<typename TList, 
template <typename, typename> typename Function, 
typename SV>
struct Reduce;

template
<template <typename, typename> typename Function,
typename SV>
struct Reduce<NullType, Function, SV>
{
    using Result = SV;
};

template 
<typename H, typename T,
template <typename, typename> typename Function,
typename SV>
struct Reduce<Typelist<H, T>, Function, SV>
{
    using Result = typename Function<H, Reduce<T, Function, SV>::Result>::Result;
};

// ------ Numeric algorithms

// auxillary function for finding maximum
template <typename SV, typename TList>
struct MaxHelper;

template <typename SV>
struct MaxHelper<SV, NullType>
{
    using Result = SV;
};

template <typename SV, typename H, typename T>
struct MaxHelper<SV, Typelist<H, T>>
{
    using Result = utils::Conditional<
    (SV::value > H::value), 
    MaxHelper<SV, T>::Result, 
    MaxHelper<H, T>::Result>::Result;
};

// find maximum
template <typename TList>
struct Max;

template <typename H, typename T>
struct Max<Typelist<H, T>>
{
    using Result = typename MaxHelper<H, T>::Result;
};

// sorting
template <typename TList>
struct Sort;

template <>
struct Sort<NullType>
{
    using Result = NullType;
};

template <typename H, typename T>
struct Sort<Typelist<H, T>>
{
    using Result = Typelist
    <
        Max<Typelist<H, T>>::Result,
        Sort<
            DeleteFirst<
                Max<Typelist<H, T>>::Result,
                Typelist<H, T>
            >::Result
        >::Result
    >;
};

}; //typelist

#endif