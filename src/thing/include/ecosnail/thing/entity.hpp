#pragma once

#include <cstdint>

namespace ecosnail::thing {

class Entity {
public:
    using ValueType = uint64_t;

    explicit Entity(ValueType id) : _id(id) {}
    operator ValueType() { return _id; }

    friend bool operator==(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id == rhs._id;
    }

    friend bool operator!=(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id != rhs._id;
    }

    friend bool operator<(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id < rhs._id;
    }

    friend bool operator>(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id > rhs._id;
    }

    friend bool operator<=(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id <= rhs._id;
    }

    friend bool operator>=(const Entity& lhs, const Entity& rhs)
    {
        return lhs._id >= rhs._id;
    }

private:
    ValueType _id;
};

} // namespace ecosnail::thing
