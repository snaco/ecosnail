#ifndef ECOSNAIL_CONTAINERS_HETERO_PACK_HPP
#define ECOSNAIL_CONTAINERS_HETERO_PACK_HPP

#include <type_traits>
#include <utility>

namespace ecosnail {
namespace containers {

namespace internal {

/**
 * Check if the first type in a type list is not contained in the rest of the
 * list.
 */

template <class T1, class T2, class... Others>
constexpr bool firstIsUnique()
{
    return !std::is_same<T1, T2>::value && firstIsUnique<T1, Others...>();
}

template <class T>
constexpr bool firstIsUnique()
{
    return true;
}

/**
 * Check if a type list contains no identical types.
 */

template <class T1, class T2, class... Others>
constexpr bool unique()
{
    return firstIsUnique<T1, T2, Others...>() && unique<T2, Others...>();
}

template <class T>
constexpr bool unique()
{
    return true;
}

template <class T>
constexpr T argOfType()
{
    static_assert(false, "argOfType: requested type not found");
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

} // namespace internal

template <class... Types>
class HeteroPack {
public:
    template <class... Args>
    HeteroPack(Args... args)
    { }

    template <class T>
    const T& at() const
    {
        static_assert(false,
            "ecosnail::containers::HeteroPack: requested type is not present");
    }

    template <class T>
    T& at()
    {
        static_assert(false,
            "ecosnail::containers::HeteroPack: requested type is not present");
    }

    template <class T>
    bool has(T value) const
    {
        return false;
    }

    template <class Other>
    bool containedIn(const Other& other) const
    {
        return true;
    }

    template <class... OtherTypes>
    bool operator==(const HeteroPack<OtherTypes...>& other) const
    {
        return other.containedIn(*this);
    }

    template <class... OtherTypes>
    bool operator!=(const HeteroPack<OtherTypes...>& other) const
    {
        return !(*this == other);
    }
};

template <class First, class... Others>
class HeteroPack<First, Others...> : HeteroPack<Others...> {
    static_assert(internal::unique<First, Others...>(),
        "ecosnail::containers::HeteroPack: "
        "cannot contain values of identical types");

public:
    template <class... Args>
    HeteroPack(Args... args)
        : _t(std::move(internal::argOfType<First>(args...)))
        , HeteroPack<Others...>(args...)
    { }

    template <class T>
    const T& at() const
    {
        return HeteroPack<Others...>::at<T>();
    }

    template <>
    const First& at<First>() const
    {
        return _t;
    }

    template <class T>
    T& at()
    {
        return HeteroPack<Others...>::at<T>();
    }

    template <>
    First& at<First>()
    {
        return _t;
    }

    template <class T>
    bool has(const T& value) const
    {
        return HeteroPack<Others...>::has<T>(value);
    }

    template <>
    bool has<First>(const First& value) const
    {
        return value == _t;
    }

    template <class Other>
    bool containedIn(const Other& other) const
    {
        return other.has(_t) && HeteroPack<Others...>::containedIn(other);
    }

    template <class... OtherTypes>
    bool operator==(const HeteroPack<OtherTypes...>& other) const
    {
        return this->containedIn(other) && other.containedIn(*this);
    }

    template <class... OtherTypes>
    bool operator!=(const HeteroPack<OtherTypes...>& other) const
    {
        return !(*this == other);
    }


private:
    First _t;
};

}} // namespace ecosnail::containers

#endif