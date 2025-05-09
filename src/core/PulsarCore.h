#pragma once
#include <string>
#include <vector>
#include <../include/toml++/toml.h>

class PulsarCore {
    std::string version;
    std::string platform;

public:
    static std::string current_path;
    static toml::table pulsar_locale;

    void set_platform(const std::string &platform);
    void set_version(const std::string &version);

    // основной функционал

    /**
     * Main pulsarvenv core func
     * @return error code
     */
    int start();



};


