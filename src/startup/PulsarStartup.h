#pragma once
#include <string>
//.
class PulsarStartup {
private:
    std::string version;



public:
    bool check_have_directories();
    std::string current_path;
    PulsarStartup();
    std::string get_version();

};
