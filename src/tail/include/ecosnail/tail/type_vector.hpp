#pragma once

#include <type_traits>

namespace ecosnail::tail {

template <class... Types> struct TypeVector {};

template <class... TypeVectorTypes, class... Types>
struct _Extend<TypeVector<TypeVectorTypes...>, Types...> {
    using Type = TypeVector<TypeVectorTypes..., Types...>;
};

template <template <class> class Condition, class... TypeVectorTypes>
struct _ExtendIf<TypeVector<TypeVectorTypes...>, Condition> {
    using Type = TypeVector<TypeVectorTypes...>;
};

template <
    class... TypeVectorTypes,
    template <class> class Condition,
    class FirstType,
    class... OtherTypes>
struct _ExtendIf<
        TypeVector<TypeVectorTypes...>,
        Condition,
        FirstType,
        OtherTypes...> {
    using Type = std::conditional_t<
        Condition<FirstType>::value,
        ExtendIf<
            TypeVector<TypeVectorTypes..., FirstType>,
            Condition,
            OtherTypes...>,
        ExtendIf<TypeVector<TypeVectorTypes...>, Condition, OtherTypes...>>;
};

template <class... TypeVectorTypes, template <class> class Condition>
struct _Filter<TypeVector<TypeVectorTypes...>, Condition> {
    using Type = ExtendIf<TypeVector<>, Condition, TypeVectorTypes...>;
};

} // namespace ecosnail::tail
