#include "CommandHandler.h"

#include <iostream>
#include <../utils/split/split.h>
#include <string>
#include <toml++/toml.h>
#include "../profile/PulsarProfileManager.h"
#include <fstream>
#include "PulsarCore.h"

using namespace std;

void CommandHandler::execute(string command) {
    string command_without_quotes = remove_quotes(command);
    vector<string> command_split = split(command_without_quotes, " ");
    if (command_split.size() >=2  || (command_split.size() >=1 && (command_split[0] == "exit" || command_split[0] == "cls" || command_split[0] == "calc"))) {
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
    }
    else {
        cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
    }
}

void CommandSetrule::execute(const vector<string> &command) {
        if (command[1] == "showWarnings") {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml" );
            if (!(command[2] == "true" || command[2] == "false")) {
                cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
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

void CommandPulsar::execute(const std::vector<std::string> &command) {
    if (command[1] == "info") {
        PulsarCurrentProfile::show_info();
    }
}
