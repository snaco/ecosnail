#pragma once

#include "tokens.hpp"
#include "objects.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace nyaa {

class Parser {
public:
    Parser(std::shared_ptr<Tokenizer> tokenizer)
        : _tokens(tokenizer)
        , _current(_tokens->get())
    { }

    Nyaa parseDocument();

private:
    void next();
    void request(Token::Type type);

    std::string readString();
    std::pair<std::string, Nyaa> readKeyValue();

    String parseString();
    List parseList();
    Dictionary parseDictionary();
    Dictionary parseFreeDictionary();
    Structure parseStructure();
    Nyaa parseObject();

    std::shared_ptr<Tokenizer> _tokens;
    Token _current;
};

} // namespace nyaa