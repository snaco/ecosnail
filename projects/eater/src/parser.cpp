#include <ecosnail/eater/parser.hpp>
#include <ecosnail/eater/builders.hpp>

void Argument::setHelpMessage(std::string&& helpMessage)
{
    _helpMessage = std::move(helpMessage);
}

std::shared_ptr<EaterBuilder> Eater::operator->()
{
    return std::make_shared<EaterBuilder>(*this);
}

Argument& Eater::addArgument(Argument arg)
{
    _args.push_back(arg);
    return _args.back();
}
