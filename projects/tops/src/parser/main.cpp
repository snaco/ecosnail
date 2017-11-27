#include "tops.hpp"

#include <iostream>

namespace et = ecosnail::tops;

int main()
{
    try {
        et::Tops document = et::parseDocument("test.tops");

        std::cout << document << std::endl;

        std::cout << document["oh my god"];
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
}