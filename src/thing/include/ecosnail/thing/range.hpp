#pragma once

#include <ecosnail/thing/entity.hpp>
#include <ecosnail/thing/component_map.hpp>

#include <functional>
#include <iterator>
#include <map>
#include <type_traits>
#include <utility>

namespace ecosnail::thing {

enum class Get : bool {
    Entities,
    Components,
};

template <class Component, bool isConst, Get get>
class SubIterator {
    using Iterator = std::conditional_t<
        isConst,
        typename ComponentMap<Component>::const_iterator,
        typename ComponentMap<Component>::iterator>;

    using ValueType = std::conditional_t<get == Get::Entities, Entity, Component>;

public:
    // TODO: make private?
    explicit SubIterator(Iterator iterator)
        : _iterator(iterator)
    { }

    SubIterator& operator++()
    {
        ++_iterator;
        return *this;
    }

    const ValueType& operator*() const
    {
        if constexpr (get == Get::Entities) {
            return _iterator->first;
        } else {
            return _iterator->second;
        }
    }

    template <class = std::enable_if_t<!isConst>>
    ValueType& operator*()
    {
        if constexpr (get == Get::Entities) {
            return _iterator->first;
        } else {
            return _iterator->second;
        }
    }

    friend bool operator==(const SubIterator& lhs, const SubIterator& rhs)
    {
        return lhs._iterator == rhs._iterator;
    }

    friend bool operator!=(const SubIterator& lhs, const SubIterator& rhs)
    {
        return lhs._iterator != rhs._iterator;
    }

private:
    Iterator _iterator;
};

template <class Component, Get get>
class Range {
public:
    // TODO: make private?
    explicit Range(ComponentMap<Component>& map) : _map(map) {}

    auto begin()
    {
        return SubIterator<Component, false, get>(_map.begin());
    }

    auto begin() const
    {
        return SubIterator<Component, true, get>(_map.begin());
    }

    auto end()
    {
        return SubIterator<Component, false, get>(_map.end());
    }

    auto end() const
    {
        return SubIterator<Component, true, get>(_map.end());
    }

private:
    ComponentMap<Component>& _map;
};

} // namespace ecosnail::thing
