#include "common.hpp"

#include <ecosnail/dimentia.hpp>

using ecosnail::dimentia::Dimented;

struct Unit {};

int main()
{
    static_assert(IsValid<Dimented, int, Unit<1>>());
    static_assert(!IsValid<Dimented, int, Unit<1>, Unit<2>>());
}
