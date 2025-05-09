#pragma once
#include <string>
#include <vector>
#include <../include//toml++/toml.hpp>
//.
class PulsarProfileManager {
private:
    toml::table setup_standart_settings();

    public:
    PulsarProfileManager();
    std::vector<std::string> account_names;
    void setup_accounts();
    void register_profile();
    void login_profile();
};

class PulsarCurrentProfile {
    public:
    static std::string name;
    static bool showWarnings;
};
