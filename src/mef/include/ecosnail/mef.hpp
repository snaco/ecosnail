#pragma once

#include <type_traits>

namespace ecosnail {
namespace mef {

/**
 * Check if a type list contains no identical types.
 */

template <class First, class... Others>
struct Unique : std::true_type {};

template <class T1, class T2, class... Others>
struct Unique<T1, T2, Others...> :
    std::bool_constant<
        !std::is_same<T1, T2>::value &&
        Unique<T1, Others...>::value &&
        Unique<T2, Others...>::value> {};

template <class Type, class... Others>
struct OneOf : std::false_type {};

template <class Type, class First, class... Others>
struct OneOf<Type, First, Others...> :
    std::bool_constant<
        std::is_same<Type, First>::value ||
        OneOf<Type, Others...>::value> {};

/**
 * Arithmetic operators result type
 */

template <class Left, class Right>
struct SumTypeHelper {
    using Type = decltype(std::declval<Left>() + std::declval<Right>());
};

template <class Left, class Right>
using SumType = typename SumTypeHelper<Left, Right>::Type;

template <class Left, class Right>
struct DiffTypeHelper {
    using Type = decltype(std::declval<Left>() - std::declval<Right>());
};

template <class Left, class Right>
using DiffType = typename DiffTypeHelper<Left, Right>::Type;

template <class Left, class Right>
struct MulTypeHelper {
    using Type = decltype(std::declval<Left>() * std::declval<Right>());
};

template <class Left, class Right>
using MulType = typename MulTypeHelper<Left, Right>::Type;

template <class Left, class Right>
struct DivTypeHelper {
    using Type = decltype(std::declval<Left>() / std::declval<Right>());
};

template <class Left, class Right>
using DivType = typename DivTypeHelper<Left, Right>::Type;

}} // namespace ecosnail::mef
