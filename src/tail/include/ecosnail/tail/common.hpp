#pragma once

namespace ecosnail::tail {

template <class, class> struct Equal;

template <class, class> struct Has;

template <class Container, class... Extensions> struct _Extend;
template <class Container, class... Extensions>
using Extend = typename _Extend<Container, Extensions...>::Type;

template <
    class Container,
    template <class> class Condition,
    class... Extensions> struct _ExtendIf;
template <
    class Container,
    template <class> class Condition,
    class... Extensions>
using ExtendIf = typename _ExtendIf<Container, Condition, Extensions...>::Type;

template <class Container, template <class> class Condition> struct _Filter;
template <class Container, template <class> class Condition>
using Filter = typename _Filter<Container, Condition>::Type;

} // namespace ecosnail::tail
