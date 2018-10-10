#pragma once

#include <ecosnail/argo/data.hpp>

#include <memory>

namespace ecosnail::argo {

template <class Type>
class OptionIterator {
public:
    const Type& operator*() const
    {
        return _data->values.at(_index);
    }

    OptionIterator& operator++()
    {
        _index++;
        return *this;
    }

    OptionIterator operator++(int)
    {
        OptionIterator previous(*this);
        operator++();
        return previous;
    }

private:
    OptionIterator(std::shared_ptr<OptionData<Type>> data, size_t index)
        : _data(std::move(data))
        , _index(index)
    { }

    const std::shared_ptr<OptionData<Type>> _data;
    size_t _index;
};

} // namespace ecosnail::argo

namespace std {

template <class Type>
struct iterator_traits<ecosnail::argo::OptionIterator<Type>> {
    using difference_type = ptrdiff_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using iterator_category = input_iterator_tag;
};

} // namespace std
