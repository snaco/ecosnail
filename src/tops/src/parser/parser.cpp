#include "parser.hpp"
#include <stdexcept>
#include <sstream>

namespace ecosnail {
namespace tops {

namespace {

void reportUnexpectedToken(const Token& token)
{
    std::ostringstream error;
    error << "Unexpected token: " << token;
    throw std::runtime_error(error.str());
}

template <class T>
Tops makeNyaa(T&& object)
{
    return Tops(
        typeid(T),
        static_cast<Object*>(new T(std::forward<T>(object))));
}

} // namespace

void Parser::request(Token::Type type)
{
    Token token = _tokens->get();
    if (token.type != type) {
        reportUnexpectedToken(token);
    }
}

bool Parser::nextTypesAre(const std::vector<Token::Type>& types)
{
    for (size_t i = 0; i < types.size(); i++) {
        if (_tokens->peek(i).type != types[i]) {
            return false;
        }
    }
    return true;        
}

Token::Type Parser::nextType()
{
    return _tokens->peek().type;
}

//
// Parse functions
//

Tops Parser::parseDocument()
{
    if (nextTypesAre({Token::Type::String, Token::Type::KeyValueSep})) {
        // Parse key-value pairs into dictionary
        Tops data;
        while (nextType() == Token::Type::String) {
            data.insert(parseKeyValuePair());
        }
        request(Token::Type::End);
        return data;
    } else {
        // Parse objects into list
        Tops data;
        while (nextType() != Token::Type::End) {
            data.push_back(parseObject());
        }
        request(Token::Type::End);
        return data;
    }
}

Tops Parser::parseObject()
{
    if (nextTypesAre({Token::Type::String, Token::Type::NameSep})) {
        std::string name = _tokens->get().string;
        _tokens->get();
        return Tops(parseObjectValue(), name);
    } else {
        return Tops(parseObjectValue());
    }
}

Tops::Value Parser::parseObjectValue()
{
    switch (_tokens->peek().type) {
        case Token::Type::String: return {parseString()};
        case Token::Type::ListStart: return {parseList()};
        case Token::Type::DictStart: return {parseDictionary()};
        default: reportUnexpectedToken(_tokens->get());
    }
    return {};
}

std::string Parser::parseString()
{
    std::string result = _tokens->get().string;
    return result;
}

std::vector<Tops> Parser::parseList()
{
    std::vector<Tops> data;

    request(Token::Type::ListStart);
    while (_tokens->peek().type != Token::Type::ListEnd) {
        Tops object = parseObject();
        data.push_back(object);
    }
    request(Token::Type::ListEnd);

    return data;
}

std::map<std::string, Tops> Parser::parseDictionary()
{
    std::map<std::string, Tops> data;

    request(Token::Type::DictStart);
    while (_tokens->peek().type != Token::Type::DictEnd) {
        std::pair<std::string, Tops> keyValuePair = parseKeyValuePair();
        data.insert(std::move(keyValuePair));
    }
    request(Token::Type::DictEnd);

    return data;
}

std::pair<std::string, Tops> Parser::parseKeyValuePair()
{
    std::string key = parseString();
    request(Token::Type::KeyValueSep);
    Tops value = parseObject();
    return std::make_pair(key, value);
}

}} // namespace ecosnail::tops