#pragma once
#include <string>
#include <vector>
#include <../include//toml++/toml.hpp>
#include <../core/PulsarCore.h>



class PulsarProfileManager {
private:
    toml::table setup_standart_settings();

    public:
    PulsarProfileManager();
    std::vector<std::string> account_names;
    void setup_accounts();
    int register_profile(std::string name);
    int login_profile(std::string name);
};

class PulsarCurrentProfile {
    public:
    static std::string name;
    static bool showWarnings;
    static bool showPath;
    static bool betaFunc;
    static void show_info();
};

class CommandProfile {
    public: 
    int execute(std::vector<std::string> &command);
};
