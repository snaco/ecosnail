#pragma once

#include <utility>
#include <type_traits>
#include <ostream>

namespace ecosnail {
namespace tail {

template <template <class> class Layout, class Type> struct Vector;

namespace internal {

struct DummyType {};

/**
 * Size
 */

template <template <class> class Data>
struct SizeHelper :
    std::integral_constant<size_t, Data<DummyType>::Layout.size()> {};

template <template <class> class Data>
inline constexpr size_t Size = SizeHelper<Data>::value;

/**
 * IndexSequence
 */

template <template <class> class Data>
struct IndexSequenceHelper {
    using Type = std::make_index_sequence<Size<Data>>;
};

template <template <class> class Data>
using IndexSequence = typename IndexSequenceHelper<Data>::Type;

/**
 * get
 */

template <size_t Index, template <class> class Data, class Type>
constexpr Type& get(Vector<Data, Type>& vector)
{
    return vector.*Data<Type>::Layout[Index];
}

template <size_t Index, template <class> class Data, class Type>
constexpr const Type& get(const Vector<Data, Type>& vector)
{
    return vector.*Data<Type>::Layout[Index];
}

template <
    size_t Index = 0, template <class> class Data, class Type, class Callable>
constexpr void forEach(Vector<Data, Type>& vector, Callable&& callable)
{
    if constexpr (Index < Size<Data>) {
        callable(get<Index>(vector));
        forEach<Index + 1>(vector, std::forward<Callable>(callable));
    }
}

template <
    size_t Index = 0,
    template <class> class Data,
    class LeftType,
    class RightType,
    class Callable>
constexpr void forEach(
    Vector<Data, LeftType>& left,
    const Vector<Data, RightType>& right,
    Callable&& callable)
{
    if constexpr (Index < Size<Data>) {
        callable(get<Index>(left), get<Index>(right));
        forEach<Index + 1>(left, right, std::forward<Callable>(callable));
    }
}

template <
    size_t Index,
    template <class> class Data,
    class LeftType,
    class RightType,
    class Operation>
auto onElement(
    const Vector<Data, LeftType>& left,
    const Vector<Data, RightType>& right,
    Operation&& operation)
{
    auto result = operation(get<Index>(left), get<Index>(right));
    return result;
}

/**
 * pairwise (v1, v2, op)
 *
 * -> r = {x1, ... x_n} : x_i = op(v1_i, v2_i)
 */

template <
    template <class> class Data,
    class LeftType,
    class RightType,
    class Operation,
    size_t... Indices>
auto pairwiseImpl(
    const Vector<Data, LeftType>& left,
    const Vector<Data, RightType>& right,
    Operation&& operation,
    std::index_sequence<Indices...>)
{
    using ResultType = decltype(
        operation(std::declval<LeftType>(), std::declval<RightType>()));
    auto result = Vector<Data, ResultType>{
        onElement<Indices>(
            left,
            right,
            std::forward<Operation>(operation))...
    };
    return result;
}

template <
    template <class> class Data,
    class LeftType,
    class RightType,
    class Operation>
auto pairwise(
    const Vector<Data, LeftType>& left,
    const Vector<Data, RightType>& right,
    Operation&& operation)
{
    return pairwiseImpl(
        left,
        right,
        std::forward<Operation>(operation),
        IndexSequence<Data>{});
}

/**
 * foreach
 */

template <
    template <class> class Data,
    class Type,
    class Operation,
    size_t... Indices>
auto foreachImpl(
    const Vector<Data, Type>& vector,
    Operation&& operation,
    std::index_sequence<Indices...>)
{
    using ResultType = decltype(operation(std::declval<Type>()));
    return Vector<Data, ResultType> {operation(get<Indices>(vector))...};
}

template <
    template <class> class Data,
    class Type,
    class Operation>
auto foreach(const Vector<Data, Type>& vector, Operation&& operation)
{
    return foreachImpl(
        vector, std::forward<Operation>(operation), IndexSequence<Data>{});
}

} // namespace internal

template <template <class> class Data, class Type>
struct Vector : Data<Type> {
    template <class... Args>
    explicit Vector(Args&&... args)
        : Data<Type>{std::forward<Args>(args)...}
    { }

    /**
     * Binary arithmetic operators
     */

    template <class OtherType>
    Vector& operator+=(const Vector<Data, OtherType>& other)
    {
        static_assert(
            std::is_convertible<OtherType, Type>(),
            "not convertible");

        internal::forEach(*this, other, [] (auto& left, const auto& right) {
            left += right;
        });
        return *this;
    }

    template <class OtherType>
    Vector& operator-=(const Vector<Data, OtherType>& other)
    {
        static_assert(
            std::is_convertible<OtherType, Type>(),
            "not convertible");

        internal::forEach(*this, other, [] (auto& left, const auto& right) {
            left -= right;
        });
        return *this;
    }

    template <class Scalar>
    Vector& operator*=(const Scalar& scalar)
    {
        static_assert(
            std::is_convertible<Scalar, Type>(),
            "not convertible");

        internal::forEach(*this, [&scalar] (auto &value) {
            value *= scalar;
        });
        return *this;
    }

    template <class Scalar>
    Vector& operator/=(const Scalar& scalar)
    {
        static_assert(
            std::is_convertible<Scalar, Type>(),
            "not convertible");

        internal::forEach(*this, [&scalar] (auto &value) {
            value /= scalar;
        });
        return *this;
    }
};

/**
 * Binary arithmetic operators
 */

template <template <class> class Data, class LeftType, class RightType>
auto operator+(
    const Vector<Data, LeftType>& left, const Vector<Data, RightType>& right)
{
    return internal::pairwise(left, right,
        [] (const auto& l, const auto& r) {
            return l + r;
        });
}

template <template <class> class Data, class LeftType, class RightType>
auto operator-(
    const Vector<Data, LeftType>& left, const Vector<Data, RightType>& right)
{
    return internal::pairwise(left, right,
        [] (const auto& l, const auto& r) {
            return l - r;
        });
}

template <template <class> class Data, class VectorType, class ScalarType>
auto operator*(const Vector<Data, VectorType>& vector, const ScalarType& scalar)
{
    return internal::foreach(vector,
        [&scalar] (const auto& v) {
            return v * scalar;
        });
}

template <template <class> class Data, class VectorType, class Scalar>
auto operator*(const Scalar& scalar, const Vector<Data, VectorType>& vector)
{
    return internal::foreach(vector,
        [&scalar] (const auto& v) {
            return v * scalar;
        });
}

template <template <class> class Data, class VectorType, class Scalar>
auto operator/(const Vector<Data, VectorType>& vector, const Scalar& scalar)
{
    return internal::foreach(vector,
        [&scalar] (const auto& v) {
            return v / scalar;
        });
}

/**
 * Stream insertion (output)
 */

namespace internal {

template <size_t Index = 1, template <class> class Data, class Type>
void printTail(std::ostream& stream, const Vector<Data, Type>& vector)
{
    if constexpr (Index < Size<Data>) {
        stream << ", " << get<Index>(vector);
        printTail<Index + 1>(stream, vector);
    }
}

} // namespace internal

template <template <class> class Data, class Type>
std::ostream& operator<<(std::ostream& stream, const Vector<Data, Type>& vector)
{
    stream << "[";
    if constexpr (internal::Size<Data> > 0) {
        stream << internal::get<0>(vector);
        internal::printTail(stream, vector);
    }
    stream << "]";
    return stream;
}

}} // namespace ecosnail::tail
