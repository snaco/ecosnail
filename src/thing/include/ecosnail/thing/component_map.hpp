#pragma once

#include <ecosnail/thing/entity.hpp>

#include <map>

namespace ecosnail::thing {

template <class Component>
using ComponentMap = std::map<Entity, Component>;

} // namespace ecosnail::thing
