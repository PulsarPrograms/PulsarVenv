#include "PulsarProfileManager.h"
#include <filesystem>
#include "../core/PulsarCore.h"
#include <iostream>
#include <string>
#include <fstream>
#include <../include/toml++/toml.hpp>
#include <../utils/other_utils/other_utils.h>

using namespace std;

string PulsarCurrentProfile::name;
bool PulsarCurrentProfile::showWarnings = true;



PulsarProfileManager::PulsarProfileManager() {}

void PulsarProfileManager::setup_accounts() {
    for (const auto& entry : filesystem::directory_iterator(PulsarCore::current_path + "\\system\\profiles")) {
        if (entry.is_directory()) {
            account_names.push_back(entry.path().filename().string());
        }
    }

}

toml::table PulsarProfileManager::setup_standart_settings() {
    toml::table config {{"showWarnings" , true}, {"name" , "none"}, {"locale" , "standard_locale.toml"}, {"path", PulsarCore::current_path}, {"theme", "standard.txt"}};
    return config;
}

void PulsarProfileManager::register_profile(string name) {
    string newProfile = PulsarCore::current_path + "\\system\\profiles\\" + name;
    filesystem::create_directory(newProfile);
    filesystem::create_directory(newProfile + "\\settings");
    toml::table config = setup_standart_settings();
    config.insert_or_assign("name", name);
    PulsarCurrentProfile::name = config["name"].value_or(name);
    PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(true);
    std::ofstream file(newProfile + "\\settings" + "\\config.toml");
    file << config;
    file.close();
}

void PulsarProfileManager::login_profile(string name) {
    for (const auto& account : account_names) {
        if (account == name) {
            string path_to_acc = PulsarCore::current_path + "\\system\\profiles\\" + name;
            toml::table config = toml::parse_file(path_to_acc + "\\settings\\config.toml");
            PulsarCurrentProfile::name = config["name"].value_or(name);
            PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(true);
        }
        else {
            cerr << PulsarCore::pulsar_locale["profile_not_found"].value_or("Profile not found") << endl;
        }
    }

}
void PulsarCurrentProfile::show_info() {
    unsigned int point_time = clock();
    cout << PulsarCore::pulsar_locale["profile_name"].value_or("ERROR: [LOCALE ERROR]") << "   " << PulsarCurrentProfile::name << endl;
    cout << PulsarCore::pulsar_locale["current_os"].value_or("ERROR: [LOCALE ERROR]") << "  " << PulsarCore::platform << endl;
    cout << PulsarCore::pulsar_locale["current_version"].value_or("ERROR: [LOCALE ERROR]") << "   " << PulsarCore::version << endl;
    cout << PulsarCore::pulsar_locale["current_time"].value_or("ERROR: [LOCALE ERROR]") << "   " << getCurrentDateTime() <<endl;
    cout << PulsarCore::pulsar_locale["work_time"].value_or("ERROR: [LOCALE ERROR]") << "   " << (point_time - PulsarCore::start_time) / 1000 << " sec" << endl;
    cout << PulsarCore::pulsar_locale["start_time"].value_or("ERROR: [LOCALE ERROR]") << "   " << PulsarCore::launch_time << endl;
}
