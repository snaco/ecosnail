#pragma once

#include <ecosnail/dimentia/dimented_decl.hpp>

#include <ecosnail/tail.hpp>

#include <type_traits>

namespace ecosnail {
namespace dimentia {
namespace internal {

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
 * EnableIfConvertible
 */

template <class From, class To> struct _IsConvertible : std::false_type {};

template <
    class FromType, class... FromDimensions,
    class ToType, class... ToDimensions>
struct _IsConvertible<
        Dimented<FromType, FromDimensions...>,
        Dimented<ToType, ToDimensions...>> :
    std::bool_constant<
        std::is_convertible_v<FromType, ToType> &&
        tail::TypeSetsEqual<
            tail::TypeSet<FromDimensions...>,
            tail::TypeSet<ToDimensions...>>> {};

template <class From, class To>
using EnableIfConvertible = std::enable_if_t<_IsConvertible<From, To>::value>;

/**
 * EnableIfSameDimensions
 */

template <class Left, class Right> struct _HaveSameDimensions;

template <
    class LeftType, class... LeftDimensions,
    class RightType, class... RightDimensions>
struct _HaveSameDimensions<
    Dimented<LeftType, LeftDimensions...>,
    Dimented<RightType, RightDimensions...>>
    : std::bool_constant<
        tail::TypeSetsEqual<
            tail::TypeSet<LeftDimensions...>,
            tail::TypeSet<RightDimensions...>>> {};

template <class Left, class Right>
inline constexpr bool HaveSameDimensions =
    _HaveSameDimensions<Left, Right>::value;

template <class Left, class Right>
using EnableIfSameDimensions =
    std::enable_if_t<_HaveSameDimensions<Left, Right>::value>;

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
        mef::SumType<LeftValueType, RightValueType>,
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
        mef::DiffType<LeftValueType, RightValueType>,
        LeftDimensions...>;
};

template <class Left, class Right>
using DiffType = typename _DiffType<Left, Right>::Type;

/**
 * MultiplyByDimension
 */

template <class DimentedType, class MulDimension>
struct _MultiplyByDimension;

template <class DimentedType, class... Dimensions, class MulDimension>
struct _MultiplyByDimension<
        Dimented<DimentedType, Dimensions...>,
        MulDimension> {
    using Type = std::conditional_t<
        MatchesOneOf<MulDimension, Dimensions...>,
        Dimented<DimentedType, AddToDimension<MulDimension, Dimensions>...>,
        Dimented<DimentedType, Dimensions..., MulDimension>>;
};

template <class DimentedType, class MulDimension>
using MultiplyByDimension =
    typename _MultiplyByDimension<DimentedType, MulDimension>::Type;

/**
 * MultiplyByDimensions
 */

template <class DimentedType, class... MulDimensions>
struct _MultiplyByDimensions {
    using Type = DimentedType;
};

template <
    class DimentedType, class FirstMulDimension, class... OtherMulDimensions>
struct _MultiplyByDimensions<
        DimentedType, FirstMulDimension, OtherMulDimensions...> {
    using Type = typename _MultiplyByDimensions<
            MultiplyByDimension<DimentedType, FirstMulDimension>,
            OtherMulDimensions...>::Type;
};

template <class DimentedType, class... MulDimensions>
using MultiplyByDimensions =
    typename _MultiplyByDimensions<DimentedType, MulDimensions...>::Type;

/**
 * DivideByDimensions
 */

template <class DimentedType, class... DivDimensions>
using DivideByDimensions =
    MultiplyByDimensions<DimentedType, InverseDimension<DivDimensions>...>;

/**
 * ProductType
 */

template <class, class> struct ProductTypeHelper;

template <class Left, class Right>
using ProductType = typename ProductTypeHelper<Left, Right>::Type;

template <
    class LeftType, class... LeftDimensions,
    class RightType, class... RightDimensions>
struct ProductTypeHelper<
        Dimented<LeftType, LeftDimensions...>,
        Dimented<RightType, RightDimensions...>> {
    using Type = MultiplyByDimensions<
        Dimented<mef::ProductType<LeftType, RightType>, LeftDimensions...>,
        RightDimensions...>;
};

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
    using Type = DivideByDimensions<
        Dimented<mef::DivisionType<LeftType, RightType>, LeftDimensions...>,
        RightDimensions...>;
};


}}} // namespace ecosnail::dimentia::internal
