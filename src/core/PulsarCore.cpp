#include "PulsarCore.h"
#include <iostream>
#include "filesystem"
#include <../include/toml++/toml.h>
#include <../profile/PulsarProfileManager.h>
#include "../commandHandler/CommandHandler.h"
#include "../utils/clearScreen/clear_screen.h"

using namespace std;

string PulsarCore::current_path = "";
toml::table PulsarCore::pulsar_locale;
string PulsarCore::version = "";
string PulsarCore::platform = "";
int PulsarCore::start_time = 0;
string PulsarCore::launch_time = "";

int PulsarCore::account_update(bool is_clear) {
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
    if (is_clear) {
        string line;
        fstream file(PulsarCore::current_path + "\\system\\themes\\" + config["theme"].value_or("standard.txt"), fstream::in | fstream::out | ios::app);
        if (!(file.is_open())) {
            cerr << pulsar_locale["file_not_exixts"].value_or("ERROR: [LOCALE ERROR]") << endl;
        }
        clear_screen();
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    return 0;
}

int PulsarCore::start() {
    clear_screen();
    string command;
    account_update(true);
    while (true) {
        cout << "$> "; getline(cin, command);
        CommandHandler::execute(command);

    }
return 0;
}
