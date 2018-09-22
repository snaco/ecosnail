#pragma once

#include <cassert>
#include <optional>

namespace ecosnail::tail {

template <class Iterator>
class KeyIterator {
public:
    KeyIterator()
    { }

    explicit KeyIterator(Iterator iterator)
        : _iterator(iterator)
    { }

    KeyIterator& operator++()
    {
        assert(_iterator);
        ++*_iterator;
        return *this;
    }

    const auto& operator*() const
    {
        return (*_iterator)->first;
    }

    friend bool operator==(const KeyIterator& lhs, const KeyIterator& rhs)
    {
        return lhs._iterator == rhs._iterator;
    }

    friend bool operator!=(const KeyIterator& lhs, const KeyIterator& rhs)
    {
        return lhs._iterator != rhs._iterator;
    }

private:
    std::optional<Iterator> _iterator;
};

template <class Iterator>
class KeyRange {
public:
    KeyRange()
    { }

    KeyRange(Iterator begin, Iterator end)
        : _iterators({begin, end})
    { }

    auto begin() const
    {
        return _iterators ?
            KeyIterator<Iterator>(_iterators->begin) :
            KeyIterator<Iterator>();
    }

    auto end() const
    {
        return _iterators ?
            KeyIterator<Iterator>(_iterators->end) :
            KeyIterator<Iterator>();
    }

private:
    struct Iterators {
        Iterator begin;
        Iterator end;
    };

    std::optional<Iterators> _iterators;
};

template <class Map>
auto keyRange(const Map& map)
{
    return KeyRange(map.begin(), map.end());
}

template <class Map>
auto keyRange()
{
    using Begin = decltype(std::declval<Map>().begin());
    using End = decltype(std::declval<Map>().end());
    static_assert(std::is_same<Begin, End>());

    return KeyRange<Begin>();
}

} // namespace ecosnail::tail
