#include <ecosnail/dimentia.hpp>

#include <type_traits>

using ecosnail::dimentia::Dimented;

template <int> struct UnitA {};
template <int> struct UnitB {};

struct LeftMock {};
struct RightMock {};

struct SumResult {};
struct DiffResult {};
struct MulResult {};
struct DivResult {};

SumResult operator+(const LeftMock&, const RightMock&) { return {}; }
DiffResult operator-(const LeftMock&, const RightMock&) { return {}; }
MulResult operator*(const LeftMock&, const RightMock&) { return {}; }
DivResult operator/(const LeftMock&, const RightMock&) { return {}; }

template <
    template <class, class> class Operation, class L, class R, class = void>
struct IsInvalid : std::true_type {};

template <template <class, class> class Operation, class L, class R>
struct IsInvalid<Operation, L, R, std::void_t<Operation<L, R>>>
    : std::false_type {};

template <
    template <class, class> class Operation, class L, class R, class Result>
struct ResultIs : std::bool_constant<
    std::is_same_v<Operation<L, R>, Result>> {};

template <class L, class R>
using Sum = decltype(std::declval<L>() + std::declval<R>());

template <class L, class R>
using Diff = decltype(std::declval<L>() - std::declval<R>());

template <class L, class R>
using Mul = decltype(std::declval<L>() * std::declval<R>());

template <class L, class R>
using Div = decltype(std::declval<L>() / std::declval<R>());

int main()
{
    using A = Dimented<LeftMock, UnitA<1>>;
    using B = Dimented<RightMock, UnitA<1>>;
    using C = Dimented<RightMock, UnitA<-1>>;
    using D = Dimented<RightMock, UnitB<1>>;

    using AB_Sum = Dimented<SumResult, UnitA<1>>;
    using AB_Diff = Dimented<DiffResult, UnitA<1>>;
    using AB_Mul = Dimented<MulResult, UnitA<2>>;
    using AC_Mul = Dimented<MulResult>;
    using AD_Mul = Dimented<MulResult, UnitA<1>, UnitB<1>>;

    static_assert(ResultIs<Sum, A, B, AB_Sum>());
    static_assert(IsInvalid<Sum, A, C>());
    static_assert(IsInvalid<Sum, A, D>());

    static_assert(ResultIs<Diff, A, B, AB_Diff>());
    static_assert(IsInvalid<Diff, A, C>());
    static_assert(IsInvalid<Diff, A, D>());

    static_assert(ResultIs<Mul, A, B, AB_Mul>());
    // TODO: fix
    //static_assert(ResultIs<Mul, A, C, AC_Mul>());
    static_assert(ResultIs<Mul, A, D, AD_Mul>());
}
