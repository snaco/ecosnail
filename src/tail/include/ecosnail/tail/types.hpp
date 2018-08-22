#pragma once

#include <type_traits>

namespace ecosnail::tail {

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

/**
 * Arithmetic operators result type
 */

template <class Left, class Right>
using SumType = decltype(std::declval<Left>() + std::declval<Right>());

template <class Left, class Right>
using DiffType = decltype(std::declval<Left>() - std::declval<Right>());

template <class Left, class Right>
using ProductType = decltype(std::declval<Left>() * std::declval<Right>());

template <class Left, class Right>
using DivisionType = decltype(std::declval<Left>() / std::declval<Right>());

} // namespace ecosnail::tail
