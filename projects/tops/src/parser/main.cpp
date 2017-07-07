#include <ecosnail/tops.hpp>

#include <iostream>

namespace et = ecosnail::tops;

int main()
{
    //try {
        et::Tops document = et::parseDocument("test.tops");

        std::cout << document.list()[0].dictionary()["color_bits"].string();

    //} catch (std::exception& e) {
    //    std::cerr << e.what();
    //    return 1;
    //}
}