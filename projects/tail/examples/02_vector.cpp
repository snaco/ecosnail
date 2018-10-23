#include <ecosnail/tail.hpp>

#include <array>
#include <iostream>

namespace et = ecosnail::tail;

template <class T>
struct XYData {
    T x;
    T y;

    static constexpr std::array<T XYData::*, 2> Layout {
        &XYData::x,
        &XYData::y
    };
};

using ScreenPoint = ecosnail::tail::Vector<XYData, int>;

int main()
{
    ScreenPoint p1 {1, 2};
    ScreenPoint p2 {3, 4};

    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;

    p1 += p2;
    std::cout << "p1 = " << p1 << std::endl;

    ScreenPoint p3 = p1 + p2;
    std::cout << "p3 = " << p3 << std::endl;

    std::cout << "p1 * 2 = " << (p1 * 2) << std::endl;
}
