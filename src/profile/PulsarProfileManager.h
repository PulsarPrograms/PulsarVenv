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
    void register_profile(std::string name);
    void login_profile(std::string name);
};

class PulsarCurrentProfile {
    public:
    static std::string name;
    static bool showWarnings;
    static void show_info();
};
