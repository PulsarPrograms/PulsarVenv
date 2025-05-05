#include "PulsarProfileManager.h"
#include <filesystem>
#include "../core/PulsarCore.h"
#include <iostream>
#include <string>
#include <fstream>
#include <../include/toml++/toml.hpp>

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
    toml::table config {{"showWarnings" , true}, {"name" , "none"}};
    return config;
}

void PulsarProfileManager::register_profile() {
    string name;
    cout << "Введите имя профиля: " << endl;
    getline(cin, name);
    string newProfile = PulsarCore::current_path + "\\system\\profiles\\" + name;
    filesystem::create_directory(newProfile);
    filesystem::create_directory(newProfile + "\\settings");
    toml::table config = setup_standart_settings();
    config.insert_or_assign("name", name);
    std::ofstream file(newProfile + "\\settings" + "\\config.toml");
    file << config;
}

void PulsarProfileManager::login_profile() {
    string name;
    cout << "Введите имя профиля: " << endl;
    for (const auto& account : account_names) {
        if (account == name) {
            string path_to_acc = PulsarCore::current_path + "\\system\\profiles\\" + name;
            auto config = toml::parse_file(path_to_acc + "\\settings\\config.toml");
            PulsarCurrentProfile::name = config["name"].value_or(name);
            PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(true);
        }
    }

}
