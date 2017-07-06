#pragma once

#include "errors.hpp"
#include "categories.hpp"
#include "tokens.hpp"
#include "traits.hpp"

#include <string>
#include <istream>
#include <sstream>
#include <type_traits>

namespace ecosnail {
namespace config {

template <class T>
void read(T &arg, std::istream &is)
{
    Tokenizer tokenizer(is);
    read(arg, tokenizer);
}

template <class T>
typename std::enable_if<
        std::is_same<
            typename category<T>::type, cat::scalar>::value &&
        !traits::is_string<T>::value >::type
    read(T &arg, Tokenizer &tok)
{
    Token token = tok.request(Scalar);
	std::istringstream stream(token.text);
    stream >> arg;
}

template <class T>
typename std::enable_if<
        std::is_same<
            typename category<T>::type, cat::scalar>::value &&
        traits::is_string<T>::value >::type
    read(T &arg, Tokenizer &tok)
{
    Token token = tok.request(Scalar);
    arg = token.text;
}

template <class T>
enable_if_category_t<T, cat::pair>
    read(T &arg, Tokenizer &tok)
{
    read(arg.first, tok);
    tok.request(DictionaryKeyValue);
    read(arg.second, tok);
}

template <class T>
enable_if_category_t<T, cat::sequence>
    read(T &arg, Tokenizer &tok)
{
    tok.request(SequenceStart);
    while (tok.peekNext().type != SequenceEnd) {
        arg.emplace_back();
        read(arg.back(), tok);
    }
    tok.request(SequenceEnd);
}

template <class T>
enable_if_category_t<T, cat::dictionary>
    read(T &arg, Tokenizer &tok)
{
    tok.request(DictionaryStart);
    while (tok.peekNext().type != DictionaryEnd) {
        typename T::key_type key;
        typename T::mapped_type value;

        read(key, tok);
        tok.request(DictionaryKeyValue);
        read(value, tok);

        arg.emplace(key, value);
    }
    tok.request(DictionaryEnd);
}


template <class T>
enable_if_category_t<T, cat::structure>
    read(T &arg, Tokenizer &tok)
{
    auto fields = arg.define_fields();

    tok.request(StructStart);
    while (tok.peekNext().type != StructEnd) {
        std::string fieldName = tok.request(Scalar).text;
        tok.request(StructNameValue);
        fields.readField(fieldName, tok);
    }
    tok.request(StructEnd);
}


template <class T>
enable_if_category_t<T, cat::unknown>
    read(T &arg, Tokenizer &tok)
{
    static_assert(std::is_same<typename category<T>::type, cat::unknown>::value,
		"ucfg: attempted to read an unsupported type");
}

}} // namespace ecosnail::config
