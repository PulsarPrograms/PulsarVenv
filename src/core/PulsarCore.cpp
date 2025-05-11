#include "PulsarCore.h"
#include <iostream>
#include "filesystem"
#include <../include/toml++/toml.h>
#include <../profile/PulsarProfileManager.h>
#include "../commandHandler/CommandHandler.h"
#include "../itils/clearScreen/clear_screen.h"

using namespace std;

string PulsarCore::current_path = "";
toml::table PulsarCore::pulsar_locale;

void PulsarCore::set_platform(const std::string &platform) {
    this->platform = platform;
}

void PulsarCore::set_version(const std::string &version) {
    this->version = version;
}

int PulsarCore::account_update() {
    if (!(filesystem::exists(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml"))) {
        cerr << PulsarCore::pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR] ")<< endl;
    }
    toml::table config = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
    if (!(filesystem::exists(PulsarCore::current_path + "\\system\\locale\\" + config["locale"].value_or("standard_locale.toml")))) {
        cerr << PulsarCore::pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR] ")<< endl;
    }
    PulsarCore::pulsar_locale = toml::parse_file(PulsarCore::current_path + "\\system\\locale\\" + config["locale"].value_or("standard_locale.toml"));
    PulsarCurrentProfile::name = config["name"].value_or(PulsarCurrentProfile::name);
    PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(PulsarCurrentProfile::showWarnings);

    return 0;
}

int PulsarCore::start() {
    clear_screen();
    string command;
    account_update();
    while (true) {
        cout << "$> "; getline(cin, command);
        CommandHandler::execute(command);

    }
return 0;
}
