#pragma once
#include <string>
#include <vector>

class CommandHandler {

public:
    static int execute(std::string command);
};


class CommandPulsar {
public:
    void execute(const std::vector<std::string> &command);

};

class CommandProfile {

};

class CommandSetrule {
public:
    void execute(const std::vector<std::string> &command);
};


class CommandConfig {
public:
    void execute(const std::vector<std::string> &command);
};

class CommandScript {
    public:
    int execute(const std::vector<std::string> &command);
};

class CommandAlias {
    public:
    int execute(const std::vector<std::string> &command);
};