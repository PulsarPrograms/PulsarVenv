#pragma once
#include <string>
#include <vector>

class CommandHandler {

public:
    static void execute(std::string command);
};


class CommandPulsar {
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