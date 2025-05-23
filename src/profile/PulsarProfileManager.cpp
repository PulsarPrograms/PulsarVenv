#include "PulsarProfileManager.h"
#include <filesystem>
#include "../core/PulsarCore.h"
#include <iostream>
#include <string>
#include <fstream>
#include <../include/toml++/toml.hpp>
#include <../utils/utils.h>
#include <iomanip>

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

int PulsarProfileManager::register_profile(string name) {
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
    toml::table alias {};
    std::ofstream file1(newProfile + "\\settings" + "\\alias.toml");
    file1 << alias;
    file1.close();
    ofstream file2(newProfile + "\\settings" + "\\log.pulslog");
    file2.close();

    return 0;
}

int PulsarProfileManager::login_profile(string name) {
    for (const auto& account : account_names) {
        if (account == name) {
            string path_to_acc = PulsarCore::current_path + "\\system\\profiles\\" + name;
            toml::table config = toml::parse_file(path_to_acc + "\\settings\\config.toml");
            PulsarCurrentProfile::name = config["name"].value_or(name);
            PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(true);
            return 0;
        }
    }
    cerr << PulsarCore::pulsar_locale["profile_not_found"].value_or("Profile not found") << endl;
    return 1;

}
void PulsarCurrentProfile::show_info() {
    unsigned int point_time = clock();
    const int label_width = 20;
    const int value_width = 30;

    auto print_row = [&](const std::string& label, const std::string& value) {
        std::cout << std::left << std::setw(label_width) << label
                  << std::left << std::setw(value_width) << value << "\n";
    };

    print_row(PulsarCore::pulsar_locale["profile_name"].value_or("ERROR:"), PulsarCurrentProfile::name);
    print_row(PulsarCore::pulsar_locale["current_os"].value_or("ERROR:"), PulsarCore::platform);
    print_row(PulsarCore::pulsar_locale["current_version"].value_or("ERROR:"), PulsarCore::version);
    print_row(PulsarCore::pulsar_locale["current_time"].value_or("ERROR:"), get_current_date_time());
    print_row(PulsarCore::pulsar_locale["work_time"].value_or("ERROR:"),
              std::to_string((point_time - PulsarCore::start_time) / 1000) + " sec");
    print_row(PulsarCore::pulsar_locale["start_time"].value_or("ERROR:"), PulsarCore::launch_time);
}