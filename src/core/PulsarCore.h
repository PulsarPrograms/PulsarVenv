#pragma once
#include <string>
#include <vector>
#include <../include/toml++/toml.h>

class PulsarCore {
    std::string version;

public:
    static std::string current_path;
    static toml::table pulsar_locale;
    static std::string platform;


    static int account_update();
    // основной функционал

    /**
     * Main pulsarvenv core func
     * @return error code
     */
    int start();



};


