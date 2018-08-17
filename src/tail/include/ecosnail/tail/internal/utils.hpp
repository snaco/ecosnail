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

}}} // namespace ecosnail::tail::internal
