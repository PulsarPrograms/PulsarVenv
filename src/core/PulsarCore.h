#pragma once
#include <string>
#include <vector>
#include <../include/toml++/toml.h>

class PulsarCore {
    int profile_start();

public:
    static std::string current_path;
    static toml::table pulsar_locale;
    static std::string platform;
    static std::string version;
    static int start_time;
    static std::string launch_time;
    static toml::table alias;


    static int account_update(bool is_clear=false);
    static int set_theme(toml::table &config, int color=7);
    // основной функционал

    /**
     * Main pulsarvenv core func
     * @return error code
     */
    int start();




};


