#include <ecosnail/dimentia.hpp>

#include <iostream>

using ecosnail::dimentia::Dimented;

template <int> struct Meters {
    static constexpr const char name[] = "m";
};

template <int> struct Seconds {};

int main()
{
    auto x = Dimented<int, Meters<1>>(10);
    auto y = Dimented<int, Meters<1>>(20);
    auto z = Dimented<int, Meters<2>>(30);

    std::cout << "x = " << x << std::endl;

    x += y;

    std::cout << "x = " << x << std::endl;

    auto s = Dimented<int, Seconds<1>>(5);

    auto p1 = Dimented<int, Meters<1>, Seconds<1>>(1);
    Dimented<int, Seconds<1>, Meters<1>> p2 = p1;
}
