#include "tops.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include <utility>
#include <fstream>
#include <stdexcept>
#include <memory>

namespace ecosnail {
namespace tops {

const char* TopsTypeName<Tops::String>::value = "string";
const char* TopsTypeName<Tops::List>::value = "list";
const char* TopsTypeName<Tops::Dictionary>::value = "dictionary";

Tops::Tops(Value value, std::string name)
    : _name(std::move(name))
    , _value(std::move(value))
{ }

void Tops::push_back(const Tops& value)
{
    return get<List>().push_back(value);
}

Tops& Tops::operator[](List::size_type pos)
{
    return get<List>()[pos];
}

std::pair<Tops::Dictionary::iterator, bool> Tops::insert(
    const Dictionary::value_type& value)
{
    return get<Dictionary>().insert(value);
}

Tops& Tops::operator[](const std::string& key)
{
    return get<Dictionary>()[key];
}

const std::string& Tops::asString()
{
    return get<String>();
}

std::ostream& operator<<(std::ostream& stream, const Tops& tops)
{
    tops.prettyPrint(stream);
    return stream;
}

void Tops::prettyPrint(std::ostream& stream, int offset, bool skipFirst) const
{
    if (!skipFirst) {
        stream << std::string(offset, ' ');
    }

    if (std::holds_alternative<std::monostate>(_value)) {
        stream << "EMPTY";
    }
    else if (std::holds_alternative<Tops::String>(_value)) {
        stream << std::get<Tops::String>(_value);
    }
    else if (std::holds_alternative<Tops::List>(_value)) {
        stream << "[" << std::endl;
        for (const Tops& tops : std::get<Tops::List>(_value)) {
            tops.prettyPrint(stream, offset + 4);
            stream << std::endl;
        }
        stream << std::string(offset, ' ') << "]";
    }
    else if (std::holds_alternative<Tops::Dictionary>(_value)) {
        stream << "{" << std::endl;
        for (const auto& pair : std::get<Tops::Dictionary>(_value)) {
            stream << std::string(offset + 4, ' ') << pair.first << " = ";
            pair.second.prettyPrint(stream, offset + 4, true);
            stream << std::endl;
        }
        stream << std::string(offset, ' ') << "}";
    }
}

Tops parseDocument(const std::string& path)
{
    std::ifstream stream(path);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open document: " + path);
    }

    auto lexer = std::make_shared<BufferedLexer>(stream);
    Parser parser(lexer);
    return parser.parseDocument();
}

}} // namespace ecosnail::tops