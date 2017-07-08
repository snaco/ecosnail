#pragma once

#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <variant>
#include <stdexcept>
#include <ostream>

namespace ecosnail {
namespace tops {

template <class T> struct TopsTypeName;

class Tops {
public:
    using String = std::string;
    using List = std::vector<Tops>;
    using Dictionary = std::map<std::string, Tops>;
    using Value = std::variant<std::monostate, String, List, Dictionary>;

    Tops() {}
    Tops(Value value, std::string name = "");

    void push_back(const Tops& value);
    Tops& operator[](List::size_type pos);

    std::pair<Dictionary::iterator, bool> insert(
        const Dictionary::value_type& value);
    Tops& operator[](const std::string& key);

    const std::string& asString();

    friend std::ostream& operator<<(std::ostream& stream, const Tops& tops);

private:
    void prettyPrint(
        std::ostream& stream,
        int offset = 0,
        bool skipFirst = false) const;

    template <class T> void requestType(const std::string& name)
    {
        if (std::holds_alternative<std::monostate>(_value)) {
            _value.emplace<T>();
        } else if (!std::holds_alternative<T>(_value)) {
            throw std::runtime_error(
                name + ": not a " + TopsTypeName<T>::value);
        }
    }

    std::string _name;
    Value _value;
};

template <> struct TopsTypeName<Tops::String> { static const char* value; };
template <> struct TopsTypeName<Tops::List> { static const char* value; };
template <> struct TopsTypeName<Tops::Dictionary> { static const char* value; };

Tops parseDocument(const std::string& path);

}} // namespace ecosnail::tops
