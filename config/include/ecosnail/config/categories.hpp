#pragma once

#include <ecosnail/config/traits.hpp>

namespace ecosnail {
namespace config {

/**
 * Categories of types that ecosnail::config recognizes
 */
namespace cat {

struct scalar     { constexpr static auto name = "scalar"; };
struct pair       { constexpr static auto name = "pair"; };
struct sequence   { constexpr static auto name = "sequence"; };
struct dictionary { constexpr static auto name = "dictionary"; };
struct structure  { constexpr static auto name = "structure"; };
struct unknown    { constexpr static auto name = "unknown"; };

} // namespace cat

/**
 * Determine type category.
 */
template <class T>
struct category :
    std::conditional< std::is_arithmetic<T>::value || traits::is_string<T>::value,
        cat::scalar,
        typename std::conditional< traits::is_pair<T>::value,
            cat::pair,
            typename std::conditional< traits::has_const_iterator<T>::value,
                typename std::conditional< traits::has_key_type<T>::value,
                    cat::dictionary,
                    cat::sequence
                >::type,
                typename std::conditional< std::is_class<T>::value,
                    cat::structure,
                    cat::unknown
                >::type
            >::type
        >::type
    >
{};

template <class T, class Category>
struct enable_if_category : std::enable_if<
    std::is_same<typename category<T>::type, Category>::value> {};

template <class T, class Category>
using enable_if_category_t = typename enable_if_category<T, Category>::type;


}} // namespace ecosnail::config
