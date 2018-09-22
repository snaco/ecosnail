#pragma once

#include <cassert>
#include <optional>
#include <type_traits>

namespace ecosnail::tail {

template <class Iterator>
class ValueIterator {
    constexpr static bool isConst =
        std::is_const<decltype(*std::declval<Iterator>())>();

public:
    ValueIterator()
    { }

    explicit ValueIterator(Iterator iterator)
        : _iterator(iterator)
    { }

    ValueIterator& operator++()
    {
        assert(_iterator);
        ++*_iterator;
        return *this;
    }

    auto& operator*() const
    {
        return (*_iterator)->second;
    }

    friend bool operator==(const ValueIterator& lhs, const ValueIterator& rhs)
    {
        return lhs._iterator == rhs._iterator;
    }

    friend bool operator!=(const ValueIterator& lhs, const ValueIterator& rhs)
    {
        return lhs._iterator != rhs._iterator;
    }

private:
    std::optional<Iterator> _iterator;
};

template <class Iterator>
class ValueRange {
    constexpr static bool isConst =
        std::is_const<decltype(*std::declval<Iterator>())>();

public:
    ValueRange()
    { }

    ValueRange(Iterator begin, Iterator end)
        : _iterators({begin, end})
    { }

    template <class = std::enable_if_t<isConst>>
    auto begin() const
    {
        return _iterators ?
            ValueIterator<Iterator>(_iterators->begin) :
            ValueIterator<Iterator>();
    }

    template <class = std::enable_if_t<!isConst>>
    auto begin()
    {
        return _iterators ?
            ValueIterator<Iterator>(_iterators->begin) :
            ValueIterator<Iterator>();
    }

    template <class = std::enable_if_t<isConst>>
    auto end() const
    {
        return _iterators ?
            ValueIterator<Iterator>(_iterators->end) :
            ValueIterator<Iterator>();
    }

    template <class = std::enable_if_t<!isConst>>
    auto end()
    {
        return _iterators ?
            ValueIterator<Iterator>(_iterators->end) :
            ValueIterator<Iterator>();
    }

private:
    struct Iterators {
        Iterator begin;
        Iterator end;
    };

    std::optional<Iterators> _iterators;
};

template <class Map>
auto valueRange(Map& map)
{
    return ValueRange(map.begin(), map.end());
}

template <class Map>
auto valueRange()
{
    using Begin = decltype(std::declval<Map>().begin());
    using End = decltype(std::declval<Map>().end());
    static_assert(std::is_same<Begin, End>());

    return ValueRange<Begin>();
}

} // namespace ecosnail::tail
