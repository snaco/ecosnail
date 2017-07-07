#pragma once

#include <ecosnail/tops.hpp>

#include "tokens.hpp"

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <any>

namespace ecosnail {
namespace tops {

class Parser {
public:
    Parser(std::shared_ptr<BufferedLexer> tokenizer)
        : _tokens(tokenizer)
    { }

    Tops parseDocument();

private:
    void request(Token::Type type);

    Token::Type Parser::nextType();
    bool nextTypesAre(const std::vector<Token::Type>& types);

    Tops parseObject();
    std::any parseObjectValue();
    std::string parseString();
    std::vector<Tops> parseList();
    std::map<std::string, Tops> parseDictionary();
    std::pair<std::string, Tops> parseKeyValuePair();

    std::shared_ptr<BufferedLexer> _tokens;
};

}} // namespace ecosnail::tops