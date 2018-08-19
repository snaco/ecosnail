#pragma once

#include <ecosnail/tail/internal/utils.hpp>

#include <ecosnail/mef.hpp>

/**
 * TypeSet.
 * I seriously don't know how it may be useful.
 */

namespace ecosnail {
namespace tail {

/**
 * TypeSet - A set of distinct types.
 *
 * A certain type may appear in a TypeSet only once. Two TypeSets are considered
 * equal when they contain the same (as in std::is_same) types, independently
 * of the order they are specified.
 */

/**
 * This template is used for empty TypeSets (declared as TypeSet<>).
 */
template <class... Types>
class TypeSet {
public:
    typedef struct {} Head;
    typedef TypeSet<> Tail;

    static constexpr bool empty() { return true; }

    /**
     * TypeSet contains type T
     */
    template <class T>
    static constexpr bool has()
    {
        return false;
    }

    /**
     * TypeSet contains every element of another TypeSet
     */
    template <class OtherTypeSet>
    static constexpr bool contains()
    {
        return OtherTypeSet::empty();
    }
};

/**
 * This template is used for TypeSets containing at least one type.
 */
template <class First, class... Others>
class TypeSet<First, Others...> {
    static_assert(mef::Unique<First, Others...>(),
        "TypeSet: all types must be different");

public:
    using Head = First;
    using Tail = TypeSet<Others...>;

    static constexpr bool empty() { return false; }

    template <class T>
    static constexpr bool has()
    {
        return std::is_same<T, First>::value || Tail::template has<T>();
    }

    template <class OtherTypeSet>
    static constexpr bool contains()
    {
        return OtherTypeSet::empty() ||
            has<typename OtherTypeSet::Head>() && contains<typename OtherTypeSet::Tail>();
    }
};

template <class LeftTypeSet, class RightTypeSet>
struct _TypeSetsEqual : std::bool_constant<
    LeftTypeSet::template contains<RightTypeSet>() &&
    RightTypeSet::template contains<LeftTypeSet>()> {};

template <class LeftTypeSet, class RightTypeSet>
inline constexpr bool TypeSetsEqual =
    _TypeSetsEqual<LeftTypeSet, RightTypeSet>::value;

template <class LeftTypeSet, class RightTypeSet>
using EnableIfTypeSetsEqual =
    std::enable_if_t<TypeSetsEqual<LeftTypeSet, RightTypeSet>>;

}} // namespace ecosnail::tail
