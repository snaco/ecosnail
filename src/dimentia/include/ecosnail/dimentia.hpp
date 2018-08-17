#pragma once

#include <ecosnail/mef.hpp>
#include <ecosnail/tail.hpp>

#include <utility>
#include <iostream>

namespace ecosnail {
namespace dimentia {

template <class Type, class... Dimentions> class Dimented;

namespace internal {

template <template <int> class Left, template <int> class Right>
struct IsSame : std::false_type {};

template <template <int> class Dimention>
struct IsSame<Dimention, Dimention> : std::true_type {};

template <template <int> class Sample, template <int> class... Dimentions>
struct IsOneOf : std::bool_constant<
    (false || ... || IsSame<Sample, Dimentions>::value)> {};

template <class From, class To> struct IsConvertible : std::false_type {};

template <
    class FromType, class... FromDimentions,
    class ToType, class... ToDimentions>
struct IsConvertible<
        Dimented<FromType, FromDimentions...>,
        Dimented<ToType, ToDimentions...>> :
    std::bool_constant<
        std::is_convertible_v<FromType, ToType> &&
        tail::TypeSetsEqual<
            tail::TypeSet<FromDimentions...>,
            tail::TypeSet<ToDimentions...>>::value> {};

template <class From, class To>
using EnableIfConvertible = std::enable_if_t<IsConvertible<From, To>::value>;

template <class Dimention> struct PrintDimention;

template <template <int> class Dimention, int Power>
struct PrintDimention<Dimention<Power>> {
    static void print(std::ostream& stream)
    {
        stream << Dimention<Power>::name;
        if constexpr (Power != 1) {
            stream << "^" << Power;
        }
    }
};

template <class Addition, class Dimention> struct AddToDimentionHelper;

template <class Addition, class Dimention>
using AddToDimention = typename AddToDimentionHelper<Addition, Dimention>::Type;

template <class, class> struct ProductHelper;

template <class Left, class Right>
using Product = typename ProductHelper<Left, Right>::Type;

} // namespace internal

template <class Type, template <int> class... Dimentions, int... Powers>
class Dimented<Type, Dimentions<Powers>...> {
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
     * Convert from a Dimented with the same dimention set (including powers)
     */

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented(const Other& other)
        : _value(other._value)
    { }

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented(const Other&& other)
        : _value(std::move(other._value))
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
        _value += other._value;
        return *this;
    }

    template <
        class Other,
        class = internal::EnableIfConvertible<Other, Dimented>>
    Dimented& operator-=(const Other& other)
    {
        _value -= other._value;
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

    /**
     * All Dimented objects have full access to each other
     */

    template <class OtherType, class... OtherDimentions>
    friend class Dimented;

    template <
        class LeftType, class... LeftDimentions,
        class RightType, class... RightDimentions>
    friend auto operator*(
        const Dimented<LeftType, LeftDimentions...>& left,
        const Dimented<RightType, RightDimentions...>& right);
};

/**
 * Arithmetic operators
 */

// NOTE: It may make sense to accept the first argument in operator+ by value,
// whenever a call to operator+= is possible. The same applies to other
// arithmetic operators.

template <
    class LeftType, class... LeftDimentions,
    class RightType, class... RightDimentions>
auto operator*(
    const Dimented<LeftType, LeftDimentions...>& left,
    const Dimented<RightType, RightDimentions...>& right)
{
    return internal::Product<
        Dimented<LeftType, LeftDimentions...>,
        Dimented<RightType, RightDimentions...>>(left._value * right._value);
}

/**
 * Output to stream
 */

template <class Type, class... Dimentions>
std::ostream& operator<<(std::ostream& stream, const Dimented<Type, Dimentions...>& dimented)
{
    stream << dimented.value();
    (internal::PrintDimention<Dimentions>::print(stream), ...);
    return stream;
}

namespace internal {

template <
    template <int> class LeftDimention, int LeftPower,
    template <int> class RightDimention, int RightPower>
struct AddToDimentionHelper<LeftDimention<LeftPower>, RightDimention<RightPower>> {
    using Type = RightDimention<RightPower>;
};

template <template <int> class CommonDimention, int LeftPower, int RightPower>
struct AddToDimentionHelper<CommonDimention<LeftPower>, CommonDimention<RightPower>> {
    using Type = CommonDimention<LeftPower + RightPower>;
};

template <
    template <int> class LeftDimention, int LeftPower,
    class RightType,
    template <int> class... RightDimentions, int... RightPowers>
struct ProductHelper<
        LeftDimention<LeftPower>,
        Dimented<RightType, RightDimentions<RightPowers>...>> {
    using Type = std::conditional_t<
        IsOneOf<LeftDimention, RightDimentions...>::value,
        Dimented<
            RightType,
            AddToDimention<
                LeftDimention<LeftPower>,
                RightDimentions<RightPowers>>...>,
        Dimented<
            RightType,
            LeftDimention<LeftPower>,
            RightDimentions<RightPowers>...>>;
};

template <class Addition, class DimentedType, class... Dimentions>
struct ProductHelper<Addition, Dimented<DimentedType, Dimentions...>> {
    using Type = Dimented<DimentedType, AddToDimention<Addition, Dimentions>...>;
};

template <class LeftType, class RightType, class... RightDimentions>
struct ProductHelper<
        Dimented<LeftType>,
        Dimented<RightType, RightDimentions...>> {
    using Type = Dimented<
        std::common_type_t<LeftType, RightType>,
        RightDimentions...>;
};

template <
    class LeftType, class LeftFirst, class... LeftOthers,
    class RightType, class... RightDimentions>
struct ProductHelper<
        Dimented<LeftType, LeftFirst, LeftOthers...>,
        Dimented<RightType, RightDimentions...>>{
    using Type = Product<
        Dimented<LeftType, LeftOthers...>,
        Product<LeftFirst, Dimented<RightType, RightDimentions...>>>;
};

} // namespace internal

}} // namespace ecosnail::dimentia
