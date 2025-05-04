#include "PulsarCore.h"
#include "filesystem"

using namespace std;

string PulsarCore::current_path = "";

void PulsarCore::set_platform(const std::string &platform) {
    this->platform = platform;
}

void PulsarCore::set_version(const std::string &version) {
    this->version = version;
}


