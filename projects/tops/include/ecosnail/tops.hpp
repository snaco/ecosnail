#pragma once

#include <any>
#include <optional>
#include <string>
#include <vector>
#include <map>

namespace ecosnail {
namespace tops {

class Tops {
public:
    Tops() {}
    explicit Tops(std::any data);
    Tops(std::any data, std::string name);

    bool hasName() const { return _name.has_value(); }
    const std::string& name() const { return *_name; }

     

private:
    std::optional<std::string> _name;
    std::any _data;
};

Tops parseDocument(const std::string& path);

}} // namespace ecosnail::tops