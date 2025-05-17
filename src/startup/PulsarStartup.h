#pragma once
#include <string>
#include <../include/toml++/toml.h>

class PulsarStartup {
private:
    std::string version;




public:
    bool check_have_directories();
    std::string current_path;
    toml::table pulsar_locale;
    int start_time;
    std::string launch_time;
    PulsarStartup();
    std::string get_version();

};
