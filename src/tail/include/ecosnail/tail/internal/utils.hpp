#pragma once

#include <type_traits>

namespace ecosnail {
namespace tail {
namespace internal {

template <class T> struct DependentFalse : std::false_type {};

template <class T>
constexpr T argOfType()
{
    static_assert(DependentFalse<T>(), "argOfType: requested type not found");
}

template <class T, class First, class... Others>
constexpr std::enable_if_t<std::is_same<T, First>::value, T>
argOfType(First first, Others... others)
{
    return first;
}

template <class T, class First, class... Others>
constexpr std::enable_if_t<!std::is_same<T, First>::value, T>
argOfType(First first, Others... others)
{
    return argOfType<T, Others...>(others...);
}

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

}}} // namespace ecosnail::tail::internal