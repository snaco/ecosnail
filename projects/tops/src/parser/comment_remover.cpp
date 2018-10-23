#include "comment_remover.hpp"

namespace ecosnail {
namespace tops {

CommentRemover::CommentRemover(std::istream& input)
    : _input(input)
{ }

int CommentRemover::getAtNextLine()
{
    int c = _input.get();
    while (c != '\n' && c != eof()) {
        c = _input.get();
    }
    if (c == '\n') {
        c = _input.get();
    }
    return c;
}

int CommentRemover::get()
{
    int c = _input.get();

    bool commentPossible = true;
    while (commentPossible) {
        if (c == '#') {
            c = getAtNextLine();
        }
        else {
            commentPossible = false;
        }
    }

    return c;
}

}} // namespace ecosnail::tops