#pragma once

#include "parser.hpp"

class ArgumentBuilder {
public:
    ArgumentBuilder(EaterBuilder* builder, Argument& arg)
        : _builder(builder), _arg(arg) {}

    ArgumentBuilder help(std::string helpMessage)
    {
        _arg.setHelpMessage(std::move(helpMessage));
        return *this;
    }

    EaterBuilder* operator->()
    {
        return _builder;
    }

    operator Eater();

private:
    Argument& _arg;
    EaterBuilder* _builder;
};

class EaterBuilder {
public:
    EaterBuilder(Eater& eater) : _eater(eater) {}

    ArgumentBuilder arg(std::string shortName)
    {
        Argument argument(shortName);
        return ArgumentBuilder(this, _eater.addArgument(argument));
    }

    Eater eater() { return _eater; }
    
private:
    Eater& _eater;
};
