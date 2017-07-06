#pragma once

#include <string>
#include <utility>
#include <type_traits>

namespace ecosnail {
namespace config {

/**
 * Custom type traits for ecosnail::config
 */
namespace traits {

/** T1 and T2 are same type, ignoring CV-qualifiers */
template <class T1, class T2>
struct is_like : std::integral_constant<
	bool,
    std::is_same<
		typename std::remove_cv<T1>::type,
		typename std::remove_cv<T2>::type
	>::value
> { };

/** T is a string */
template <class T>
struct is_string : std::integral_constant<
	bool,
    is_like<std::string, T>::value ||
		std::is_pointer<typename std::decay<T>::type>::value &&
			std::is_same<
				typename std::remove_cv<
					typename std::remove_pointer<
						typename std::decay<T>::type
					>::type
				>::type,
				char
			>::value
> { };

/** T is a scalar value */
template <class T>
struct is_scalar : std::integral_constant<
	bool,
    std::is_arithmetic<T>::value ||
		is_string<T>::value
> { };

template <class T>
struct is_pair : std::false_type {};

template <class Key, class Value>
struct is_pair< std::pair<Key, Value> > : std::true_type {};

/**
 * Helper template for has_* traits.
 *
 * A simple struct template that defines void as ::type. The template,
 * obviously, may be instantiated for valid types only.
 *
 * It is typically used in template specialization of has_* structures. See
 * GENERATE_HAS_TYPE macro for comments on usage.
 */
template <class T>
struct void_if_type { typedef void type; };

/**
 * Generate traits detecting if a child type is defined inside T.
 *
 * This family of templates is used to detect if the type being tested contains
 * certain child types (e.g. key_type, value_type, const_iterator). This helps
 * to determine if we are dealing with a container, and what type of container.
 *
 * Each child type to detect requires a separate template, and so it is using
 * a separate macro call.
 *
 * The base definition accepts a type argument which defaults to void, and is
 * implemented as std::false_type.
 *
 * The specialization provides void_if_type result as this parameter, and is
 * implemented as std::true_type. If the child type is indeed present, the
 * parameter is void, and the specialization is chosen. Otherwise, the
 * specialization is invalid, and so the base template definition is used.
 *
 * It is crucial that this second parameter type is the same (void) for the base
 * template and its specialization. This way, specialization participates in
 * the ordering, and may be selected.
 */
#define GENERATE_HAS_TYPE(SUBTYPE)                                          \
    template <class T, class Value = void>                                  \
    struct has_ ## SUBTYPE : std::false_type {};                           	\
                                                                            \
    template <class T>                                                      \
    struct has_ ## SUBTYPE <                                                \
        T, typename void_if_type<typename T::SUBTYPE>::type>                \
    : std::true_type {};

GENERATE_HAS_TYPE(const_iterator)
GENERATE_HAS_TYPE(key_type)

#undef GENERATE_HAS_TYPE

}}} // namespace ecosnail::config::traits

