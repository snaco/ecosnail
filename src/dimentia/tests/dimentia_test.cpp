#include <ecosnail/dimentia.hpp>

#include <iostream>

using ecosnail::dimentia::Dimented;

template <int> struct Dim1 {
    static constexpr const char name[] = "D1";
};
template <int> struct Dim2 {
    static constexpr const char name[] = "D2";
};

int main()
{
    Dimented<int, Dim1<1>> x(10);
    Dimented<int, Dim2<2>> y(10);
    auto z = x * y;

    static_assert(std::is_same_v<decltype(x * y), Dimented<int, Dim1<1>, Dim2<2>>>);

    std::cout << z << std::endl;
}
