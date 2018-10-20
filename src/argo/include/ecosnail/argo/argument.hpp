#pragma once

#include <ecosnail/argo/errors.hpp>

#include <iterator>
#include <memory>
#include <vector>
#include <sstream>

namespace ecosnail::argo {

template <class T>
T cast(std::string_view text)
{
    std::stringstream stream;
    stream << text;
    T value;
    stream >> value;
    return value;
}

struct ArgumentData {
    virtual void provide(std::string_view value) = 0;

    bool multi = false;
    bool required = true;
    bool takesArgument = true;
    size_t timesUsed = 0;
    std::string helpText;
};

template <class T>
struct TypedArgumentData : ArgumentData {
    void provide(std::string_view value) override
    {
        values.push_back(cast<T>(value));
    }

    std::vector<T> values;
};

template <class T>
class ArgumentIterator {
public:
    explicit ArgumentIterator(
            typename std::vector<T>::const_iterator dataIterator)
        : _dataIterator(std::move(dataIterator))
    { }

    const T& operator*() const
    {
        return *_dataIterator;
    }

    ArgumentIterator& operator++()
    {
        ++_dataIterator;
        return *this;
    }

    ArgumentIterator operator++(int)
    {
        ArgumentIterator previous(*this);
        operator++();
        return previous;
    }

    friend bool operator!=(
        const ArgumentIterator& lhs, const ArgumentIterator& rhs)
    {
        return lhs._dataIterator != rhs._dataIterator;
    }

private:
    typename std::vector<T>::const_iterator _dataIterator;
};

template <class T>
class Argument {
    using Values = std::vector<T>;

public:
    explicit Argument(std::shared_ptr<TypedArgumentData<T>> data)
        : _data(std::move(data))
    { }

    // TODO: const& ?
    operator T()
    {
        check(!_data->multi, "cannot cast multi argument to a single value");
        check(!_data->values.empty(), "cannot cast, no values provided");
        return _data->values.front();
    }

    auto begin() const
    {
        check(_data->multi, "cannot iterate over a non-multi argument");
        return ArgumentIterator<T>(_data->values.begin());
    }

    auto end() const
    {
        check(_data->multi, "cannot iterator over a non-multi argument");
        return ArgumentIterator<T>(_data->values.end());
    }

    Argument help(std::string_view helpText)
    {
        _data->helpText = helpText;
        return *this;
    }

    Argument required()
    {
        _data->required = true;
        return *this;
    }

    Argument multi()
    {
        _data->multi = true;
        return *this;
    }

    Argument flag()
    {
        _data->takesArgument = false;
        return *this;
    }

    friend std::ostream& operator<<(
        std::ostream& output, const Argument& argument)
    {
        const auto& values = argument._data->values;
        if (auto it = values.begin(); it != values.end()) {
            output << *it++;
            for (; it != values.end(); ++it) {
                output << ", " << *it;
            }
        }
        return output;
    }

private:
    std::shared_ptr<TypedArgumentData<T>> _data;
};

} // namespace ecosnail::argo

namespace std {

template <class T>
struct iterator_traits<ecosnail::argo::ArgumentIterator<T>> {
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = input_iterator_tag;
};

} // namespace std
