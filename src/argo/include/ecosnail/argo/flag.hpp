#pragma once

#include <ecosnail/argo/data.hpp>
#include <ecosnail/argo/errors.hpp>

#include <memory>
#include <string_view>

namespace ecosnail::argo {

class Flag {
public:
    Flag help(std::string_view helpText)
    {
        _data->helpText = helpText;
        return *this;
    }

    Flag multi()
    {
        _data->multi = true;
        return *this;
    }

    operator bool() const
    {
        return _data->timesUsed > 0;
    }

    operator size_t() const
    {
        check(_data->multi);
        return _data->timesUsed;
    }

private:
    // TODO: provide reference (left or right) based on usage?
    Flag(std::shared_ptr<FlagData> data)
        : _data(std::move(data))
    { }

    std::shared_ptr<FlagData> _data;

    friend class Parser;
};

} // namespace ecosnail::argo
