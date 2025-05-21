#include "CommandHandler.h"

#include <iostream>
#include <../utils/utils.h>
#include <string>
#include <toml++/toml.h>
#include "../profile/PulsarProfileManager.h"
#include <fstream>
#include "PulsarCore.h"

using namespace std;

int CommandHandler::execute(string command) {
    string command_without_quotes = remove_quotes(command);
    vector<string> command_split = split(command_without_quotes);
    if (command_split.size() ==1 && command_split[0] == "pulserror") {
        cerr << PulsarCore::pulsar_locale["not_critical_error"].value_or("ERROR: [LOCALE ERROR] ") << endl;
        return 111; // Не критичная ошибка
    }
    if (command_split.size() >=2  || (command_split.size() >=1 && (command_split[0] == "exit" || command_split[0] == "clear" || command_split[0] == "calc" || command_split[0] == "out"))) {
        if (command_split[0] == "pulsar") {
            CommandPulsar pulsar_com;
            pulsar_com.execute(command_split);
        }
        else if (command_split[0] == "setrule") {
            CommandSetrule command_setrule;
            command_setrule.execute(command_split);
        }
        else if (command_split[0] == "config") {
            CommandConfig command_config;
            command_config.execute(command_split);
        }
        else if (command_split[0] == "calc") {
            string exten = (PulsarCore::platform == "Windows") ? ".exe" : ".deb";
            string calc_arg = (command_split.size() >= 2) ? command_split[1] : "";
            string commandSys ="cd " + PulsarCore::current_path + "\\system\\systemmodules && pulsarcalc" + exten + " " + calc_arg;
            system(commandSys.c_str());
        }
        else if (command_split[0] == "clear") {
           PulsarCore::account_update(true);
        }
        else if (command_split[0] == "exit") {
            return 101; //код выхода
        }
        else if (command_split[0] == "out") {
            for (size_t i = 1; i < command_split.size(); ++i) {
                cout << command_split[i];
                if (i != command_split.size() - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    else {
        cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
    }
    return 0;
}

void CommandSetrule::execute(const vector<string> &command) {
        if (command[1] == "showWarnings") {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml" );
            if (!(command[2] == "true" || command[2] == "false")) {
                cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                return;
            }
            conf.insert_or_assign("showWarnings", command[2] == "true");
            ofstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update();
        }
        else if (command[1] == "setLocale") {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml" );
            conf.insert_or_assign("locale", command[2]);
            ofstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update();
        }
        else if (command[1] == "setTheme") {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml" );
            conf.insert_or_assign("theme", command[2]);
            ofstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update(true);
        }
    }


void CommandConfig::execute(const vector<string> &command) {
    if (command[1] == "show") {
        string show_line;
        ifstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml");
        while (getline(file,show_line)) {
            cout << show_line << endl;
        }
    }
}

void CommandPulsar::execute(const vector<string>& command) {
    if (command[1] == "info") {
        PulsarCurrentProfile::show_info();
    }
}
