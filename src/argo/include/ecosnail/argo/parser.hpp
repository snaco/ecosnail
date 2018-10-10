#pragma once

#include <ecosnail/argo/data.hpp>
#include <ecosnail/argo/flag.hpp>
#include <ecosnail/argo/option.hpp>

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <memory>

namespace ecosnail::argo {

// TODO: Check flag correctness

class Parser {
public:
    template <
        class... Flags,
        class = std::enable_if_t<
            std::conjunction_v<
                std::is_convertible<Flags, std::string_view>...>>>
    Flag flag(Flags&&... flags)
    {
        auto data = std::make_shared<FlagData>();
        (addMapForLastData(flags), ...);
    }

    template <
        class Type,
        class... Flags,
        class = std::enable_if_t<
            std::conjunction_v<
                std::is_convertible<Flags, std::string_view>...>>>
    Option<Type> option(Flags&&... flags)
    {
        auto data = std::make_shared<OptionData<Type>>();
        (addMapForLastData(flags), ...);
    }

    void parse(const std::vector<std::string>& args)
    {
        for (auto arg = args.begin(); arg != args.end(); ++arg) {
            if (auto it = _flags.find(*arg); it != _flags.end()) {
                auto& flagData = it->second;

                check(flagData->multi || flagData->timesUsed == 0,
                    "a non-multi flag used multiple times: " + *arg);
                flagData->timesUsed++;
            }
            if (auto it = _options.find(*arg); it != _options.end()) {
                auto &optionData = it->second;

                check(optionData->multi || optionData->values.empty(),
                    "a non-multi option used multiple times: " + *arg);
                ++arg;
                check(arg != args.end(),
                    "argument non provided for option: " + *arg);
                optionData->values.push_back(*arg);
            }
        }

        for (const auto& [flag, optionData] : _options) {
            check(!optionData->required || !optionData->values.empty(),
                "required option not provided: " + flag);
        }
    }

private:
    void addFlagData(
        std::string_view flag, const std::shared_ptr<FlagData>& data)
    {
        if (_flags.count(flag) || _options.count(flag)) {
            throw Exception("Flag already used: "s + flag);
        }
        _flags[flag] = data;
    }

    void addOptionData(
        std::string_view flag, const std::shared_ptr<BaseOptionData>& data)
    {
        if (_flags.count(flag) || _options.count(flag)) {
            throw Exception("Flag already used: "s + flag);
        }
        _options[flag] = data;
    }

    std::map<std::string, std::shared_ptr<FlagData>> _flags;
    std::map<std::string, std::shared_ptr<BaseOptionData>> _options;
    std::vector<std::string> _freeArgs;
};

} // namespace ecosnail::argo
