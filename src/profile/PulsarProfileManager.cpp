#include "PulsarProfileManager.h"
#include <filesystem>
#include "../core/PulsarCore.h"
#include <iostream>
#include <string>
#include <fstream>
#include <../include/toml++/toml.hpp>

using namespace std;

string PulsarCurrentProfile::name = "";
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
    toml::table config {{"showWarnings" , true}};
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
    std::ofstream file(newProfile + "\\settings" + "\\config.toml");
    file << config;
}

void PulsarProfileManager::login_profile() {
    string name;
    cout << "Введите имя профиля: " << endl;
    /*В будущем добавлю логин*/
}