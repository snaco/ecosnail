#include "objects.hpp"

#include <utility>

namespace nyaa {

Nyaa::Nyaa()
    : _typeInfo(nullptr)
    , _ptr(nullptr)
{ }

Nyaa::Nyaa(const std::type_info& type, Object* ptr)
    : _typeInfo(&type)
    , _ptr(ptr)
{ }

Nyaa::Nyaa(Nyaa&& other)
    : _typeInfo(other._typeInfo)
    , _ptr(std::move(other._ptr))
{ }

Nyaa& Nyaa::operator=(Nyaa&& other) noexcept
{
    std::swap(_typeInfo, other._typeInfo);
    std::swap(_ptr, other._ptr);
    return *this;
}

} // namespace nyaa