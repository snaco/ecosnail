#pragma once

#include <type_traits>

template <template <class...> class Template, class... Types, class = void>
struct IsValid : std::false_type {};

template <template <class...> class Template, class... Types>
struct IsValid<Template, Types..., std::void_t<Template<Types...>>>
    : std::true_type {};
