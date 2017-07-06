#include "nyaason.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include <fstream>
#include <stdexcept>

namespace nyaa {

Nyaa parse(const char* fileName)
{
    std::ifstream stream(fileName);
    if (!stream.is_open()) {
        throw std::runtime_error(
            "Failed to open input file: " + std::string(fileName));
    }

    return parse(stream);
}

Nyaa parse(std::istream& stream)
{
    auto lexer = std::make_shared<Tokenizer>(stream);
    Parser parser(lexer);
    return parser.parseDocument();
}

} // namespace nyaa