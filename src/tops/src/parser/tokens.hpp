#pragma once

#include "comment_remover.hpp"

#include <string>
#include <istream>
#include <ostream>
#include <memory>
#include <utility>
#include <typeinfo>
#include <deque>

namespace ecosnail {
namespace tops {

struct Token {
    enum class Type {
        ListStart,
        ListEnd,
        DictStart,
        DictEnd,
        KeyValueSep,
        NameSep,
        String,
        End,
    };

    explicit Token(Type type)
        : type(type) {}

    Token(Type type, std::string string)
        : type(type), string(std::move(string)) {}

    Type type;
    std::string string;
};

std::ostream& operator<<(std::ostream& stream, const Token& token);

class Lexer {
public:
    Lexer(std::istream& input);
     
    Token get();

private:
    static auto eof()
    {
        return std::istream::traits_type::eof();
    }

    void next() { _current = _input.get(); }
    void skipWhitespace();

    std::string fetchStringUntil(char end);
    std::string fetchPlainString();

    CommentRemover _input;
    int _current;
};

class BufferedLexer {
public:
    BufferedLexer(std::istream& input);

    Token get();
    const Token& peek(size_t offset = 0);
    
private:
    Lexer _lexer;
    std::deque<Token> _buffer;
};

}} // namespace ecosnail::tops