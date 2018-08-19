#pragma once

#include <iostream>
#include <type_traits>

namespace ecosnail {
namespace dimentia {
namespace internal {

/**
 * IsSameUnit
 */

template <template <int> class Left, template <int> class Right>
struct _IsSameUnit : std::false_type {};

template <template <int> class DimensionTemplate>
struct _IsSameUnit<DimensionTemplate, DimensionTemplate> : std::true_type {};

template <template <int> class Left, template <int> class Right>
inline constexpr bool IsSameUnit = _IsSameUnit<Left, Right>::value;

/**
 * HaveSameUnit
 */

template <class LeftDimension, class RightDimension> struct _HaveSameUnit;

template <
    template <int> class LeftUnit, int LeftPower,
    template <int> class RightUnit, int RightPower>
struct _HaveSameUnit<LeftUnit<LeftPower>, RightUnit<RightPower>>
    : std::bool_constant<IsSameUnit<LeftUnit, RightUnit>> {};

template <class LeftDimension, class RightDimension>
inline constexpr bool HaveSameUnit =
    _HaveSameUnit<LeftDimension, RightDimension>::value;

/**
 * MatchesOneOf
 */

template <class SampleDimension, class... Dimensions>
struct _MatchesOneOf : std::bool_constant<
    (false || ... || HaveSameUnit<SampleDimension, Dimensions>)> {};

template <class SampleDimension, class... Dimensions>
inline constexpr bool MatchesOneOf =
    _MatchesOneOf<SampleDimension, Dimensions...>::value;

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

}}} // namespace ecosnail::dimentia::internal
