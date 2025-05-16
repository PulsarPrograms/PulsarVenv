#pragma once
#include <string>
#include <vector>
#include <../include/toml++/toml.h>

class PulsarCore {

public:
    static std::string current_path;
    static toml::table pulsar_locale;
    static std::string platform;
    static std::string version;
    static bool is_run_terminal;
    static std::vector<std::string> run_terminal_command;


    static int account_update();
    // основной функционал

    /**
     * Main pulsarvenv core func
     * @return error code
     */
    int start();



};


