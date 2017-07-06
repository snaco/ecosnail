#pragma once

#include "objects.hpp"

namespace nyaa {

Nyaa parse(const char* fileName);
Nyaa parse(std::istream& stream);

}