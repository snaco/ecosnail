#include <ecosnail/eater/builders.hpp>

ArgumentBuilder::operator Eater()
{
    return _builder->eater();
}

