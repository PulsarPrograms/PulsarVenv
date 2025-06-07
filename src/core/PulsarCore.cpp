#include "PulsarCore.h"
#include <iostream>
#include "filesystem"
#include <../include/toml++/toml.h>
#include <../profile/PulsarProfileManager.h>
#include "../commandHandler/CommandHandler.h"
#include "../utils/utils.h"
#include "../filesystem/PulsarFilesystem.h"

using namespace std;

string PulsarCore::current_path = "";
toml::table PulsarCore::pulsar_locale;
string PulsarCore::version = "";
string PulsarCore::platform = "";
int PulsarCore::start_time = 0;
string PulsarCore::launch_time = "";
toml::table PulsarCore::alias;


int PulsarCore::set_theme(toml::table &config, int color) {
    string line;
    fstream file(PulsarCore::current_path + "/system/themes/" + config["theme"].value_or("standard.txt"), fstream::in | fstream::out | ios::app);
    if (!(file.is_open())) {
        set_color(12);cerr << pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;set_color(7);
    }
    clear_screen();
    set_color(color);
    while (getline(file, line)) {
        cout << line << endl;
    }
    set_color(7);
    file.close();
    return 0;


}




int PulsarCore::account_update(bool is_clear) {
    if (!(filesystem::exists(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/alias.toml"))) {
        cerr << PulsarCore::pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR] ") <<endl;
        return 1;
    }
    PulsarCore::alias = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/alias.toml");

    if (!(filesystem::exists(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml"))) {
        cerr << PulsarCore::pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR] ") <<endl;
        return 1;
    }
    toml::table config = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
    if (!(filesystem::exists(PulsarCore::current_path + "/system/locale/" + config["locale"].value_or("standard_locale.toml")))) {
        cerr << PulsarCore::pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR] ") << endl;
        return 1;
    }
    PulsarCore::pulsar_locale = toml::parse_file(PulsarCore::current_path + "/system/locale/" + config["locale"].value_or("standard_locale.toml"));
    PulsarCurrentProfile::name = config["name"].value_or(PulsarCurrentProfile::name);
    PulsarCurrentProfile::showWarnings = config["showWarnings"].value_or(PulsarCurrentProfile::showWarnings);
    PulsarCurrentProfile::betaFunc = config["betaFunc"].value_or(false);
    PulsarCurrentProfile::showPath = config["showPath"].value_or(true);
    if (is_clear) {
        set_theme(config, config["themeColor"].value_or(7));
    }
    return 0;
}

int PulsarCore::profile_start() {
    clear_screen();
    if (account_update(true)!= 0) {
        return 1;
    }
    return 0;
}


int PulsarCore::start() {
    CommandLog::write_in_log(" %% start pulsar", true, "SYSTEM & Profile: " + PulsarCurrentProfile::name , false);
    if (profile_start() != 0) {
        return 1;
    }
    string command;
    while (true) {
        if (PulsarCurrentProfile::showPath ) {
            string delimiter = (PulsarCore::platform == "Windows") ? "\\" : "/";
            string base_path = filesystem::current_path().string() + delimiter + "system"+ delimiter + "pulsfs" + delimiter + "pulsarvenv" + delimiter + "home";
            string cur_path_str = PulsarFilesystem::cur_path.string();

            size_t pos = cur_path_str.find(base_path);
            if (pos != string::npos) {
                cur_path_str.replace(pos, base_path.length(), "/home");
            }

            std::replace(cur_path_str.begin(), cur_path_str.end(), '\\', '/');

            if (cur_path_str.back() != '/') {
                cur_path_str += '/';
            }

            string vis = cur_path_str;
            cout << "[ "; set_color(9); cout << vis; set_color(7); cout << " ] "; 
        }
        cout << "$> "; getline(cin, command);
        // Результат выполнения и обработка
        int result = CommandHandler::execute(command);
        if (result  != 0 && result != 101 && result != 111) {
            CommandLog::write_in_log("Critical error", true, "SYSTEM & Profile: " + PulsarCurrentProfile::name , false);
            return 1;
        }
        else if (result == 101) {
            CommandLog::write_in_log(" %% registry exit pulsar", true, "SYSTEM & Profile: " + PulsarCurrentProfile::name ,  false);
            return 0;
        }
        else if (result == 111) {
            CommandLog::write_in_log("Not critical error", true, "SYSTEM & Profile: " + PulsarCurrentProfile::name , false);
            continue;
        }
    }
return 0;
}
