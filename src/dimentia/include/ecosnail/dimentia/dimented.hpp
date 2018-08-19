#pragma once

#include <ecosnail/dimentia/dimension.hpp>
#include <ecosnail/dimentia/dimented_traits.hpp>

#include <ecosnail/mef.hpp>
#include <ecosnail/tail.hpp>

#include <utility>
#include <iostream>

namespace ecosnail {
namespace dimentia {

template <class Type, template <int> class... Units, int... Powers>
class Dimented<Type, Units<Powers>...> {
public:
    /**
     * Construction
     */

    Dimented()
        : _value()
    { }

    explicit Dimented(Type value)
        : _value(std::move(value))
    { }

    /**
     * Convert from a Dimented with the same dimension set (including powers)
     */

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented(const Other& other)
        : _value(other.value())
    { }

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented(const Other&& other)
        : _value(std::move(other.value()))
    { }

    /**
     * Get raw value
     */

    constexpr const Type& value() const
    {
        return _value;
    }

    /**
     * Arithmetic operators
     */

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented& operator+=(const Other& other)
    {
        _value += other.value();
        return *this;
    }

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented& operator-=(const Other& other)
    {
        _value -= other.value();
        return *this;
    }

    template <
        class Scalar,
        class = std::enable_if_t<std::is_convertible_v<Scalar, Type>>>
    Dimented& operator*=(const Scalar& scalar)
    {
        _value *= scalar;
        return *this;
    }

    template <
        class Scalar,
        class = std::enable_if_t<std::is_convertible_v<Scalar, Type>>>
    Dimented& operator/=(const Scalar& scalar)
    {
        _value /= scalar;
        return *this;
    }

private:
    Type _value;
};

/**
 * Arithmetic operators
 */

// NOTE: It may make sense to accept the first argument in operator+ by value,
// whenever a call to operator+= is possible. The same applies to other
// arithmetic operators.

template <
    class Left, class Right,
    class = internal::EnableIfSameDimensions<Left, Right>>
auto operator+(const Left& left, const Right& right)
{
    return internal::SumType<Left, Right>(left.value() + right.value());
}

template <
    class Left, class Right,
    class = internal::EnableIfSameDimensions<Left, Right>>
auto operator-(const Left& left, const Right& right)
{
    return internal::DiffType<Left, Right>(left.value() - right.value());
}

template <
    class Left, class Right,
    class = internal::EnableIfDimented<Left, Right>>
auto operator*(const Left& left, const Right& right)
{
    return internal::ProductType<Left, Right>(left.value() * right.value());
}

template <
    class Left, class Right,
    class = internal::EnableIfDimented<Left, Right>>
auto operator/(const Left& left, const Right& right)
{
    return internal::DivisionType<Left, Right>(left.value() / right.value());
}

/**
 * Output to stream
 */

template <class Type, class... Dimensions>
std::ostream& operator<<(
    std::ostream& stream, const Dimented<Type, Dimensions...>& dimented)
{
    stream << dimented.value();
    (internal::PrintDimension<Dimensions>::print(stream), ...);
    return stream;
}

}} // namespace ecosnail::dimentia
