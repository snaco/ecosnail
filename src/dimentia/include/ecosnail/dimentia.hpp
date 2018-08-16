#pragma once

#include <ecosnail/mef.hpp>
#include <ecosnail/tail.hpp>

#include <utility>
#include <iostream>

namespace ecosnail {
namespace dimentia {

template <class Type, class... Dimentions> class Dimented;

namespace internal {

template <class Type, class... Others>
struct IsOneOf : std::false_type {};

template <class Type, class First, class... Others>
struct IsOneOf<Type, First, Others...> :
    std::bool_constant<
        std::is_same<Type, First>::value ||
        IsOneOf<Type, Others...>::value> {};

template <class From, class To> struct IsConvertible;

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

} // namespace internal

template <class Type, template <int> class... Dimentions, int... Powers>
class Dimented<Type, Dimentions<Powers>...> {
public:
    explicit Dimented(Type value)
        : _value(std::move(value))
    { }

    constexpr const Type& value() const
    {
        return _value;
    }

    /**
     * Arithmetic operators
     */

    template <class OtherType, class... OtherPowers>
    Dimented& operator+=(const Dimented<OtherType, OtherPowers...>& other)
    {
        static_assert(
            internal::IsConvertible<
                Dimented<OtherType, OtherPowers...>, Dimented>::value);

        _value += other._value;
        return *this;
    }

private:
    Type _value;
};

template <class Type, class... Dimentions>
std::ostream& operator<<(std::ostream& stream, const Dimented<Type, Dimentions...>& dimented)
{
    stream << dimented.value();
    (internal::PrintDimention<Dimentions>::print(stream), ...);
    return stream;
}

}} // namespace ecosnail::dimentia
