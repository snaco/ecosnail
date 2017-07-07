#include <ecosnail/tops.hpp>

#include "tokens.hpp"
#include "parser.hpp"

#include <utility>
#include <fstream>
#include <stdexcept>
#include <memory>

namespace ecosnail {
namespace tops {

Tops::Tops(std::any data)
    : _data(std::move(data))
{ }

Tops::Tops(std::any data, std::string name)
    : _data(std::move(data))
    , _name(std::move(name))
{ }

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

std::string Tops::string()
{
    return std::any_cast<std::string>(_data);
}

std::vector<Tops> Tops::list()
{
    return std::any_cast<std::vector<Tops>>(_data);
}

std::map<std::string, Tops> Tops::dictionary()
{
    return std::any_cast<std::map<std::string, Tops>>(_data);
}

}} // namespace ecosnail::tops