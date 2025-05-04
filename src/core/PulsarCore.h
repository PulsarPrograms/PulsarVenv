#pragma once
#include <string>
#include <vector>

class PulsarCore {
    std::string version;
    std::string platform;

    public:
    static std::string current_path;
    void set_platform(const std::string &platform);
    void set_version(const std::string &version);



};


