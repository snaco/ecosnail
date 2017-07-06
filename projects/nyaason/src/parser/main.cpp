#include "nyaason.hpp"

#include <exception>
#include <iostream>

int main()
{
    try {
        auto nyaa = nyaa::parse("test.nyaa");
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
}