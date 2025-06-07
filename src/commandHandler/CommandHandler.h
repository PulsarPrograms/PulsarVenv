#pragma once
#include <string>
#include <vector>
#include <../profile/PulsarProfileManager.h>

class CommandHandler {

public:
    static int execute(std::string command);
};


class CommandPulsar {
private:

    std::vector<std::string> com{"info", "list"};
public:
    void execute(const std::vector<std::string> &command);

};


class CommandSetrule {
private:
    std::vector<std::string> com{
        "setLocale",
        "showWarnings",
        "setTheme" ,
        "BetaFunc",
        "themeColor",
        "list"
    };
public:
    void execute(const std::vector<std::string> &command);
};


class CommandConfig {
private:
    std::vector<std::string> com{
        "show",
        "list"
    };
public:
    void execute(const std::vector<std::string> &command);
};

class CommandScript {
private:
    std::vector<std::string> com{
        "start",
        "list"
    };
    public:
    int execute(const std::vector<std::string> &command);
};

class CommandAlias {
private:
    std::vector<std::string> com{
        "create",
        "remove",
        "show",
        "list"
    };
    public:
    int execute(const std::vector<std::string> &command);
};

class CommandLog {

public:
    static int write_in_log(const std::string text, const bool is_time, const std::string  username=PulsarCurrentProfile::name ,const bool is_trunc=false );
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