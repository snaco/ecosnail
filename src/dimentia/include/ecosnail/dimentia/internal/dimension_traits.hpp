#pragma once

#include <ecosnail/tail.hpp>

#include <iostream>
#include <type_traits>

namespace ecosnail {
namespace dimentia {
namespace internal {

/**
 * IsDimension
 */

template <class Type> struct IsDimension : std::false_type {};

template <template <int> class Unit, int Power>
struct IsDimension<Unit<Power>> : std::true_type {};

/**
 * HasNonZeroPower
 */

template <class Type> struct HasNonZeroPower;

template <template <int> class Unit, int Power>
struct HasNonZeroPower<Unit<Power>> : std::bool_constant<Power != 0> {};

/**
 * HaveSameUnit
 */

template <class LeftDimension, class RightDimension>
struct HaveSameUnit : std::false_type {
    static_assert(IsDimension<LeftDimension>());
    static_assert(IsDimension<RightDimension>());
};

template <template <int> class Unit, int LeftPower, int RightPower>
struct HaveSameUnit<Unit<LeftPower>, Unit<RightPower>>
    : std::true_type {};

/**
 * MatchesOneOf
 */

template <class SampleDimension, class... Dimensions>
struct MatchesOneOf : std::disjunction<
    std::false_type, HaveSameUnit<SampleDimension, Dimensions>...> {};

/**
 * InverseDimension
 */

template <class Dimension> struct _InverseDimention;

template <template <int> class Unit, int Power>
struct _InverseDimention<Unit<Power>> {
    using Type = Unit<-Power>;
};

template <class Dimension>
using InverseDimension = typename _InverseDimention<Dimension>::Type;

/**
 * AddToDimension
 */

template <class Addition, class Dimension> struct _AddToDimension;

template <
    template <int> class LeftDimension, int LeftPower,
    template <int> class RightDimension, int RightPower>
struct _AddToDimension<LeftDimension<LeftPower>, RightDimension<RightPower>> {
    using Type = RightDimension<RightPower>;
};

template <template <int> class CommonDimension, int LeftPower, int RightPower>
struct _AddToDimension<CommonDimension<LeftPower>, CommonDimension<RightPower>> {
    using Type = CommonDimension<LeftPower + RightPower>;
};

template <class Addition, class Dimension>
using AddToDimension = typename _AddToDimension<Addition, Dimension>::Type;

/**
 * PrintDimension
 */

template <class Dimension> struct PrintDimension;

template <template <int> class DimensionTemplate, int Power>
struct PrintDimension<DimensionTemplate<Power>> {
    static void print(std::ostream& stream)
    {
        stream << DimensionTemplate<Power>::name;
        if constexpr (Power != 1) {
            stream << "^" << Power;
        }
    }
};

/**
 * MultiplyByDimension
 */

template <class DimensionsVector, class Dimension> struct _MultiplyByDimension;

template <class... Dimensions, class Dimension>
struct _MultiplyByDimension<tail::TypeVector<Dimensions...>, Dimension> {
    using Type = std::conditional_t<
        MatchesOneOf<Dimension, Dimensions...>::value,
        tail::TypeVector<AddToDimension<Dimension, Dimensions>...>,
        tail::TypeVector<Dimensions..., Dimension>>;
};

template <class DimensionsVector, class Dimension>
using MultiplyByDimension =
    typename _MultiplyByDimension<DimensionsVector, Dimension>::Type;

/**
 * MultiplyByDimensions
 */

template <class DimensionsVector, class... Dimensions> struct _MultiplyByDimensions;

template <class DimensionsVector>
struct _MultiplyByDimensions<DimensionsVector> {
    using Type = tail::Filter<DimensionsVector, HasNonZeroPower>;
};

template <
    class DimensionsVector, class FirstDimension, class... OtherDimensions>
struct _MultiplyByDimensions<
        DimensionsVector, FirstDimension, OtherDimensions...> {
    using Type = typename _MultiplyByDimensions<
        MultiplyByDimension<DimensionsVector, FirstDimension>,
        OtherDimensions...>::Type;
};

template <class DimensionsVector, class... Dimensions>
using MultiplyByDimensions =
    typename _MultiplyByDimensions<DimensionsVector, Dimensions...>::Type;

/**
 * DivideByDimensions
 */

template <class DimensionsVector, class... Dimensions>
using DivideByDimensions =
    typename MultiplyByDimensions<DimensionsVector, InverseDimension<Dimensions>...>::Type;

}}} // namespace ecosnail::dimentia::internal
