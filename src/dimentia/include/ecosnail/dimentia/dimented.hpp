#pragma once

#include <ecosnail/dimentia/internal/dimension_traits.hpp>

#include <ecosnail/tail.hpp>

#include <utility>
#include <iostream>

namespace ecosnail::dimentia {

template <class Type, class... Dimensions> class Dimented;

namespace internal {

/**
 * EnableIfConvertible
 */

template <class From, class To> struct _IsConvertible : std::false_type {};

template <
    class FromType, class... FromDimensions,
    class ToType, class... ToDimensions>
struct _IsConvertible<
        Dimented<FromType, FromDimensions...>,
        Dimented<ToType, ToDimensions...>> :
    std::conjunction<
        std::is_convertible<FromType, ToType>,
        tail::Equal<
            tail::TypeSet<FromDimensions...>,
            tail::TypeSet<ToDimensions...>>> {};

template <class From, class To>
using EnableIfConvertible = std::enable_if_t<_IsConvertible<From, To>::value>;

} // namespace internal

template <class Type, class... Dimensions>
class Dimented {
    static_assert(std::conjunction<internal::IsDimension<Dimensions>...>());
    static_assert(std::conjunction<internal::HasNonZeroPower<Dimensions>...>());

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
        class Other, class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented(const Other& other)
        : _value(other.value())
    { }

    template <
        class Other, class = internal::EnableIfConvertible<Other, Dimented>>
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
        class Other, class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented& operator+=(const Other& other)
    {
        _value += other.value();
        return *this;
    }

    template <
        class Other, class = internal::EnableIfConvertible<Other, Dimented>>
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

namespace internal {

/**
 * MakeDimented
 */

template <class ValueType, class DimensionsVector> struct _MakeDimented;

template <class ValueType, class... Dimensions>
struct _MakeDimented<ValueType, tail::TypeVector<Dimensions...>> {
    using Type = Dimented<ValueType, Dimensions...>;
};

template <class ValueType, class DimensionsVector>
using MakeDimented = typename _MakeDimented<ValueType, DimensionsVector>::Type;

/**
 * EnableIfDimented
 */

template <class Type> struct _IsDimented : std::false_type {};

template <class ValueType, class... Dimensions>
struct _IsDimented<Dimented<ValueType, Dimensions...>> : std::true_type {};

template <class First, class... Others>
using EnableIfDimented = std::enable_if_t<
    std::conjunction_v<_IsDimented<First>, _IsDimented<Others>...>>;

/**
 * EnableIfSameDimensions
 */

template <class Left, class Right> struct HaveSameDimensions;

template <
    class LeftType, class... LeftDimensions,
    class RightType, class... RightDimensions>
struct HaveSameDimensions<
    Dimented<LeftType, LeftDimensions...>,
    Dimented<RightType, RightDimensions...>>
    :
    tail::Equal<
        tail::TypeSet<LeftDimensions...>,
        tail::TypeSet<RightDimensions...>> {};

template <class Left, class Right>
using EnableIfSameDimensions =
    std::enable_if_t<HaveSameDimensions<Left, Right>::value>;

/**
 * SumType
 */

template <class Left, class Right, class = EnableIfSameDimensions<Left, Right>>
struct _SumType;

template <
    class LeftValueType, class... LeftDimensions,
    class RightValueType, class... RightDimensions>
struct _SumType<
        Dimented<LeftValueType, LeftDimensions...>,
        Dimented<RightValueType, RightDimensions...>> {
    using Type = Dimented<
        tail::SumType<LeftValueType, RightValueType>,
        LeftDimensions...>;
};

template <class Left, class Right>
using SumType = typename _SumType<Left, Right>::Type;

/**
 * DiffType
 */

template <class Left, class Right, class = EnableIfSameDimensions<Left, Right>>
struct _DiffType;

template <
    class LeftValueType, class... LeftDimensions,
    class RightValueType, class... RightDimensions>
struct _DiffType<
        Dimented<LeftValueType, LeftDimensions...>,
        Dimented<RightValueType, RightDimensions...>> {
    using Type = Dimented<
        tail::DiffType<LeftValueType, RightValueType>,
        LeftDimensions...>;
};

template <class Left, class Right>
using DiffType = typename _DiffType<Left, Right>::Type;

/**
 * ProductType
 */

template <class, class> struct _ProductType;

template <
    class LeftType, class... LeftDimensions,
    class RightType, class... RightDimensions>
struct _ProductType<
        Dimented<LeftType, LeftDimensions...>,
        Dimented<RightType, RightDimensions...>> {
    using Type = MakeDimented<
        tail::ProductType<LeftType, RightType>,
        MultiplyByDimensions<
            tail::TypeVector<LeftDimensions...>, RightDimensions...>>;
};

template <class Left, class Right>
using ProductType = typename _ProductType<Left, Right>::Type;

/**
 * DivisionType implementation
 */

template <class, class> struct _DivisionType;

template <class Left, class Right>
using DivisionType = typename _DivisionType<Left, Right>::Type;

template <
    class LeftType, class... LeftDimensions,
    class RightType, class... RightDimensions>
struct _DivisionType<
        Dimented<LeftType, LeftDimensions...>,
        Dimented<RightType, RightDimensions...>> {
    using Type = MakeDimented<
        tail::DivisionType<LeftType, RightType>,
        DivideByDimensions<
            tail::TypeVector<LeftDimensions...>, RightDimensions...>>;
};

} // namespace internal

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

} // namespace ecosnail::dimentia
