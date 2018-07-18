#pragma once

#include <type_traits>

namespace ecosnail {
namespace tail {
namespace internal {

template <class T> struct DependentFalse : std::false_type {};

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


}}} // namespace ecosnail::tail::internal
