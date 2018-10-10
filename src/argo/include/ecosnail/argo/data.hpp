#pragma once

#include <vector>
#include <string>
#include <optional>
#include <string_view>

namespace ecosnail::argo {

// TODO: various checks for correctness

struct FlagData {
    std::string helpText;
    bool multi = false;
    size_t timesUsed = 0;
};

struct BaseOptionData {
    virtual ~BaseOptionData() {}
    virtual void provide(std::string_view value) = 0;

    std::string helpText;
    bool required = false;
    bool multi = false;
};

template <class Type>
struct OptionData : BaseOptionData {
    void provide(std::string_view value) override
    {
        values.push_back(value);
    }

    std::vector<Type> values;
};

} // namespace ecosnail::argo
