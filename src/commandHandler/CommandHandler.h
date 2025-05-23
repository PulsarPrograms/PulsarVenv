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

class CommandLog {

public:
    static int write_in_log(const std::string text, bool is_time, bool is_trunc=false);
    int execute(const std::vector<std::string> &command);
};

class CommandUtils {
private:
    static int start_point_time;
    static int end_point_time;
    static int last_total_time;

    public:
    int execute(const std::vector<std::string> &command);
};