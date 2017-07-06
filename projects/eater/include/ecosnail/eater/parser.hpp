#pragma once

#include <memory>
#include <utility>
#include <string>
#include <vector>

class EaterBuilder;

class Argument {
public:
    Argument(std::string shortName) {}

    void setHelpMessage(std::string&& helpMessage);

private:
    std::string _shortName;
    std::string _longName;
    std::string _helpMessage;
};

class Eater {
public:
    Argument& addArgument(Argument arg);

    std::shared_ptr<EaterBuilder> operator->();

private:
    std::vector<Argument> _args;
};
