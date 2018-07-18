#pragma once

#include <ecosnail/tail/internal/utils.hpp>

#include <utility>

namespace ecosnail {
namespace tail {

template <class... Types>
class HeteroPack {
public:
    template <class... Args>
    HeteroPack(Args&&... args)
    {

    }

    template <class T>
    const T& at() const
    {
        static_assert(internal::DependentFalse<T>(),
            "ecosnail::tail::HeteroPack: requested type is not present");
    }

    template <class T>
    T& at()
    {
        static_assert(internal::DependentFalse<T>(),
            "ecosnail::tail::HeteroPack: requested type is not present");
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

    template <class Callable, class... Args>
    void call(Callable&& callable, Args&&... values)
    {
        callable(std::forward<Args>(values)...);
    }
};

template <class First, class... Others>
class HeteroPack<First, Others...> : HeteroPack<Others...> {
    static_assert(internal::Unique<First, Others...>(),
        "ecosnail::tail::HeteroPack: "
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
        if constexpr (std::is_same<T, First>()) {
            return _t;
        } else {
            return HeteroPack<Others...>::template at<T>();
        }
    }

    template <class T>
    T& at()
    {
        if constexpr (std::is_same<T, First>()) {
            return _t;
        } else {
            return HeteroPack<Others...>::template at<T>();
        }
    }

    template <class T>
    bool has(const T& value) const
    {
        if constexpr (std::is_same<T, First>()) {
            return value == _t;
        } else {
            return HeteroPack<Others...>::template has<T>(value);
        }
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

    template <class Callable, class... Args>
    void call(Callable&& callable, Args&&... values)
    {
        HeteroPack<Others...>::call(std::forward<Callable>(callable), std::forward<Args>(values)..., _t);
    }

private:
    First _t;
};

}} // namespace ecosnail::tail
