#pragma once

#include "errors.hpp"

#include <sstream>
#include <exception>
#include <istream>
#include <ostream>
#include <queue>
#include <cctype>

namespace ecosnail {
namespace config {

enum TokenType {
    Scalar,

    SequenceStart,
    SequenceEnd,

    DictionaryStart,
    DictionaryKeyValue,
    DictionaryEnd,

    StructStart,
    StructNameValue,
    StructEnd,

    End,
};

struct Token {
	//Token(TokenType type, std::string text)
	//	: type(std::move(type)), text(std::move(text))
	//{ }
    
    Token(const TokenType &type, const std::string &text)
        : type(type), text(text)
    { }

    TokenType type;
    std::string text;
};

std::ostream & operator<<(std::ostream &os, const Token &token)
{
    switch (token.type) {
        case Scalar:      		 os << "scalar"; break;
        case SequenceStart:    	 os << "[ (start of sequence)"; break;
        case SequenceEnd:      	 os << "] (end of sequence)"; break;
        case DictionaryStart:    os << "{ (start of dictionary)"; break;
        case DictionaryKeyValue: os << "=> (dictionary value for key)"; break;
        case DictionaryEnd:      os << "} (end of dictionary)"; break;
        case StructStart: 		 os << "< (start of structure)"; break;
        case StructNameValue:    os << "= (value of structure field)"; break;
        case StructEnd:   		 os << "> (end of structure)"; break;
        case End:         		 os << "end"; break;
    }
    return os;
}

class UnexpectedToken : public std::exception {
public:
    UnexpectedToken(
            Token token,
            TokenType expectedTokenType,
            unsigned long long line,
            unsigned long long column)
        : std::exception()
        , _token(std::move(token))
        , _expectedType(expectedTokenType)
		, _line(line)
		, _column(column)
    { }

    ~UnexpectedToken() throw() { }

    const char * what() const throw() /*override*/
    {
        std::ostringstream oss;
        oss << _line << ":" << _column << ": expected: " << _expectedType << std::endl <<
            "    got: " << _token.text << std::endl <<
            "    which is a: " << _token.type << std::endl;
        return oss.str().c_str();
    }

private:
    Token _token;
    TokenType _expectedType;
    unsigned long long _line;
    unsigned long long _column;
};



namespace {

// Check if c is a token separator (or "space", in terms of tokenizer).
bool isSpace(int c)
{
    // TODO: Maybe replace with custom space definition. If not, maybe pay
    // attention to C++ locales (isspace from <locale>, not <cctype>). All this
    // isspace stuff is bullshit.
    return std::isspace(c) != 0;
}

} // namespace


class Tokenizer {
public:
    Tokenizer(std::istream &is)
        : _is(is)
        , _line(0)
        , _column(0)
    { }

    Token get()
    {
        if (!_buffer.empty()) {
            //Token token = std::move(_buffer.front());
            Token token = _buffer.front();
            _buffer.pop();
            return token;
        }

        int c = nextChar();
        while (isSpace(c)) {
            c = nextChar();
        }

        if (c == std::istream::traits_type::eof()) {
            return Token(End, "");
        }

        if (c == '[') return Token(SequenceStart, "[");
        if (c == ']') return Token(SequenceEnd, "]");
        if (c == '{') return Token(DictionaryStart, "{");
        if (c == '}') return Token(DictionaryEnd, "}");
        if (c == '<') return Token(StructStart, "<");
        if (c == '>') return Token(StructEnd, ">");

        if (c == '=') {
            c = nextChar();
            if (c == '>') {
                return Token(DictionaryKeyValue, "=>");
            } else {
                _is.putback(c);
                return Token(StructNameValue, "=");
            }
        }

        std::stringstream stream;
        if (c == '"') {
            c = nextChar();
            while (c != '"') {
                if (c == '\\') {
                    c = nextChar();
                    if (c == '\\' || c == '"') {
                        stream << (char)c;
                        c = nextChar();
                    } else {
                        throw ReadError("Unexpected escape sequence in string");
                    }
                } else {
                    stream << (char)c;
                    c = nextChar();
                }
            }
        } else {
            while (!std::isspace(c) &&
                    c != '[' && c != ']' &&
                    c != '{' && c != '}' &&
                    c != '<' && c != '>' &&
                    c != '=' ) {
                stream << (char)c;
                c = nextChar();
            }

            _is.putback(c);
        }
        return Token(
            Scalar,
            //std::move(stream.str())
            stream.str()
        );
    }

    Token request(TokenType type)
    {
        Token token = get();
        if (token.type != type) {
            throw UnexpectedToken(
                //std::move(token),
                token,
                type, _line, _column);
        }
        return token;
    }

    const Token & peekNext()
    {
        if (!_buffer.empty()) {
            return _buffer.front();
        }

        _buffer.push(get());
        return _buffer.front();
    }

private:
    int nextChar()
    {
        int c = getRawChar();
        if (c == '#') {
            c = getRawChar();
            while (c != '\n' && c != std::istream::traits_type::eof()) {
                c = getRawChar();
            }
        }
        return c;
    }

    // Read next character from input stream, as-is. Track current position in
    // file.
    int getRawChar()
    {
        if (_eolRead) {
            _eolRead = false;
            _line++;
            _column = 1;
        }
        int c = _is.get();
        if (c == '\n') {
            _eolRead = true;
        }
        return c;
    }

    std::istream &_is;
    bool _eolRead;
    long long _line;
    long long _column;
    std::queue<Token> _buffer;
};


}} // namespace ecosnail::config
