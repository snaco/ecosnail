#pragma once

#include <ecosnail/argo/data.hpp>
#include <ecosnail/argo/errors.hpp>
#include <ecosnail/argo/option_iterator.hpp>

#include <memory>
#include <string_view>

namespace ecosnail::argo {

class Parser;

template <class Type>
class Option {
public:
    operator const Type&()
    {
        check(!_data->multi);
        check(_data->values.size() == 1);
        return _data->values.front();
    }

    Option help(std::string_view helpText)
    {
        _data->helpText = helpText;
        return *this;
    }

    Option required()
    {
        _data->required = true;
        return *this;
    }

    Option multi()
    {
        _data->multi = true;
        return *this;
    }

    OptionIterator<Type> begin() const
    {
        check(_data->multi);
        return OptionIterator<Type>(_data, 0);
    }

    OptionIterator<Type> end() const
    {
        check(_data->multi);
        return OptionIterator<Type>(_data, _data->values.size());
    }

private:
    Option(std::shared_ptr<OptionData<Type>> data)
        : _data(std::move(data))
    { }

    std::shared_ptr<OptionData<Type>> _data;

    friend class Parser;
};


} // namespace ecosnail::argo

