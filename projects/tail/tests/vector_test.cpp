#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/tail/vector.hpp>

#include <array>

namespace et = ecosnail::tail;

template <class T>
struct XYData {
    T x;
    T y;

    static constexpr std::array Layout {
        &XYData::x,
        &XYData::y,
    };
};

using XYVectorInt = et::Vector<XYData, int>;
using XYVectorDouble = et::Vector<XYData, double>;

template <class... Types>
struct TypeList {};

template <class Type>
void checkOperation()
{
    using Vector = et::Vector<XYData, Type>;
    Vector vector {static_cast<Type>(1), static_cast<Type>(2)};
    CHECK(vector.x == 1);
    CHECK(vector.y == 2);

    vector += vector;
    CHECK(vector.x == 2);
    CHECK(vector.y == 4);
}

template <class... Types>
void checkOperations()
{
    (checkOperation<Types>(), ...);
}

TEST_CASE("arithmetic operators")
{
    checkOperations<int, double>();
}
