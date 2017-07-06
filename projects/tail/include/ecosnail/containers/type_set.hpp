/**
 * TypeSet.
 * I seriously don't know how it may be useful.
 */

#ifndef ECOSNAIL_CONTAINERS_TYPE_SET_HPP
#define ECOSNAIL_CONTAINERS_TYPE_SET_HPP

namespace ecosnail {
namespace containers {

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
    static_assert(internal::unique<First, Others...>(),
        "TypeSet: all types must be different");

public:
    typedef First Head;
    typedef TypeSet<Others...> Tail;

    static constexpr bool empty() { return false; }

    template <class T>
    static constexpr bool has()
    {
        return std::is_same<T, First>::value || Tail::has<T>();
    }

    template <class OtherTypeSet>
    static constexpr bool contains()
    {
        return OtherTypeSet::empty() ||
            has<OtherTypeSet::Head>() && contains<OtherTypeSet::Tail>();
    }
};

template <class TypeSet1, class TypeSet2>
constexpr bool typeSetsEqual()
{
    return TypeSet1::contains<TypeSet2>() && TypeSet2::contains<TypeSet1>();
}

}} // namespace ecosnail::containers

#endif