#include "PulsarCore.h"
#include <iostream>
#include "filesystem"
#include <../include/toml++/toml.h>
#include <../profile/PulsarProfileManager.h>

using namespace std;

string PulsarCore::current_path = "";
toml::table PulsarCore::pulsar_locale;

void PulsarCore::set_platform(const std::string &platform) {
    this->platform = platform;
}

void PulsarCore::set_version(const std::string &version) {
    this->version = version;
}

int account_update() {
    toml::table config = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
    PulsarCore::pulsar_locale = toml::parse_file(PulsarCore::current_path + "\\system\\locale\\" + config["locale"].value_or("standard_locale.toml"));
    return 0;
}

int PulsarCore::start() {
    string command;
    account_update();
    while (true) {
        cout << "$> "; getline(cin, command);
        if (command == "exit") {return 1;}
    }
return 0;
}
