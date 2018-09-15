#include <ecosnail/fuzz/defunction.hpp>

#include <utility>
#include <functional>
#include <iostream>

using namespace ecosnail;
using tail::HeteroPack;

template <class R, class... Args, class Callable>
auto defunction(Callable callable)
{
    return [&callable] (auto... args) {
        HeteroPack<Args...> pack(args...);
        return pack.call(callable);
    };
}

auto sum = defunction<float, int, float>(
    [] (int a, float b) -> float {
        return a + b;
    });

int main()
{
    sum(1, 2.f);
    sum(2.f, 1);
}