#include "tokens.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

namespace ecosnail {
namespace tops {

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
    static const std::map<Token::Type, std::string> typeNames{
        { Token::Type::ListStart,   "ListStart"   },
        { Token::Type::ListEnd,     "ListEnd"     },
        { Token::Type::DictStart,   "DictStart"   },
        { Token::Type::DictEnd,     "DictEnd"     },
        { Token::Type::KeyValueSep, "KeyValueSep" },
        { Token::Type::NameSep,     "NameSep"     },
        { Token::Type::String,      "String"      },
        { Token::Type::End,         "End"         },
    };

    auto typeNamePair = typeNames.find(token.type);
    if (typeNamePair == typeNames.end()) {
        throw std::runtime_error("tokenTypeName: unknown token type");
    }

    stream << typeNamePair->second << " -- " << token.string;
    return stream;
}

Lexer::Lexer(std::istream& input)
    : _input(input)
{
    next();
}

void Lexer::skipWhitespace()
{
    while (std::isspace(_current)) {
        next();
    }
}

std::string Lexer::fetchStringUntil(const char end)
{
    std::ostringstream value;
    next();
    for (; _current != end; next()) {
        if (_current == '\\') {
            next();
            if (_current == '\\') value << '\\';
            else if (_current == end) value << static_cast<char>(end);
            else if (_current == 'b') value << '\b';
            else if (_current == 'f') value << '\f';
            else if (_current == 'n') value << '\n';
            else if (_current == 'r') value << '\r';
            else if (_current == 't') value << '\t';
            else {
                throw std::runtime_error("Unknown escape sequence in string");
            }
        } else {
            value << static_cast<char>(_current);
        }
    }
    next();
    return value.str();
}

std::string Lexer::fetchPlainString()
{
    auto isPlainStringChar = [this](int c) {
        std::set<char> forbiddenChars {
            '\'', '"', '[', ']', '{', '}', ':', '=',
        };
        return c != eof() && !std::isspace(c) &&
            forbiddenChars.find(static_cast<char>(c)) == forbiddenChars.end();
    };

    std::ostringstream stream;
    while (isPlainStringChar(_current)) {
        stream << static_cast<char>(_current);
        next();
    }
    return stream.str();
}

Token Lexer::get()
{
    // Skip to next meaningful part. 
    skipWhitespace();
    if (_current == eof()) {
        return Token(Token::Type::End);
    }

    std::map<char, Token::Type> singleCharTokenTypes {
        { '[', Token::Type::ListStart },
        { ']', Token::Type::ListEnd },
        { '{', Token::Type::DictStart },
        { '}', Token::Type::DictEnd },
        { ':', Token::Type::NameSep },
        { '=', Token::Type::KeyValueSep },
    };
    
    auto singleCharTokenPair = singleCharTokenTypes.find(_current);
    if (singleCharTokenPair != singleCharTokenTypes.end()) {
        char symbol = _current;
        next();
        return { singleCharTokenPair->second, std::string(1, symbol) };
    }

    if (_current == '\'' || _current == '"') {
        std::string value = fetchStringUntil(_current);
        return { Token::Type::String, std::move(value) };
    }

    // Non-quoted text. It is either a number, or a plain string.
    std::string plainString = fetchPlainString();
    return { Token::Type::String, std::move(plainString) };
}

BufferedLexer::BufferedLexer(std::istream& input)
    : _lexer(input)
{ }

Token BufferedLexer::get()
{
    if (!_buffer.empty()) {
        Token token = std::move(_buffer.front());
        _buffer.pop_front();
        return token;
    } else {
        return _lexer.get();
    }
}

const Token& BufferedLexer::peek(size_t offset)
{
    if (offset + 1 > _buffer.size()) {
        for (size_t i = 0; i < offset + 1 - _buffer.size(); i++) {
            _buffer.push_back(_lexer.get());
        }
    }

    return _buffer[offset];
}

}} // namespace ecosnail::tops