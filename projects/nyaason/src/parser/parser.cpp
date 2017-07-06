#include "parser.hpp"
#include <stdexcept>
#include <sstream>

namespace nyaa {

namespace {

void reportUnexpectedToken(const Token& token)
{
    std::ostringstream error;
    error << "Unexpected token: " << token;
    throw std::runtime_error(error.str());
}

template <class T>
Nyaa makeNyaa(T&& object)
{
    return Nyaa(
        typeid(T),
        static_cast<Object*>(new T(std::forward<T>(object))));
}

} // namespace

void Parser::next()
{
    _current = _tokens->get();
}

void Parser::request(Token::Type type)
{
    if (_current.type != type) {
        reportUnexpectedToken(_current);
    }
    next();
}

std::string Parser::readString()
{
    if (_current.type != Token::Type::String) {
        reportUnexpectedToken(_current);
    }
    std::string value = _current.string;
    next();

    return value;
}

std::pair<std::string, Nyaa> Parser::readKeyValue()
{
    std::pair<std::string, Nyaa> keyValue;

    keyValue.first = readString();
    request(Token::Type::KeyValueSep);
    keyValue.second = parseObject();

    return std::move(keyValue);
}

String Parser::parseString()
{
    String string;
    string.value = readString();
    return string;
}

List Parser::parseList()
{
    List list;

    request(Token::Type::ListStart);
    while (_current.type != Token::Type::ListEnd) {
        list.value.push_back(parseObject());
    }
    request(Token::Type::ListEnd);
    
    return list;
}

Dictionary Parser::parseDictionary()
{
    Dictionary dict;

    request(Token::Type::DictStart);
    while (_current.type != Token::Type::DictEnd) {
        dict.value.insert(readKeyValue());
    }
    request(Token::Type::DictEnd);

    return dict;
}

Dictionary Parser::parseFreeDictionary()
{
    Dictionary dict;

    while (_current.type != Token::Type::End) {
        dict.value.insert(readKeyValue());
    }

    return dict;
}

Structure Parser::parseStructure()
{
    Structure structure;

    request(Token::Type::KeyValueSep);
    structure.name = readString();
    structure.fields = parseDictionary();

    return structure;
}

Nyaa Parser::parseObject()
{
    switch (_current.type) {
        case Token::Type::ListStart: return makeNyaa(parseList());
        case Token::Type::DictStart: return makeNyaa(parseDictionary());
        case Token::Type::KeyValueSep: return makeNyaa(parseStructure());
        case Token::Type::String: return makeNyaa(parseString());
    }
    reportUnexpectedToken(_current);
    return Nyaa();
}

Nyaa Parser::parseDocument()
{
    Nyaa document;
    switch (_current.type) {
        case Token::Type::ListStart:
            document = makeNyaa(parseList());
            break;

        case Token::Type::DictStart:
            document = makeNyaa(parseDictionary());
            break;

        case Token::Type::KeyValueSep:
            document = makeNyaa(parseStructure());
            break;

        case Token::Type::String:
            document = makeNyaa(parseFreeDictionary());
            break;

        case Token::Type::End: break;

        default:
            throw std::runtime_error("Error at token: " + _current.string);
    }

    request(Token::Type::End);
    return document;
}

} // namespace nyaa