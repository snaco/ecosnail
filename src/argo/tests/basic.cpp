#include <ecosnail/argo.hpp>
#include <ecosnail/tail.hpp>

#include <optional>

namespace argo = ecosnail::argo;

int main(int argc, char* argv[])
{
    argo::Parser parser;
    auto message = parser.option<std::string>("-m", "--message");
    auto times = parser.option<int>("-t", "--times")
        .required()
        .help("number of times to print the message");
    auto makeOffset = parser.flat("--offset")
        .help("make fancy offset");
    auto decoration = parser.option<char>("-d", "--decoration")
        .multi()
        .help("characters to decorate text with");
    parser.parse(argc, argv);
}

