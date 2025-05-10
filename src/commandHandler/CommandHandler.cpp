#include "CommandHandler.h"

#include <iostream>
#include <../itils/split/split.h>
#include <string>
#include <toml++/toml.h>
#include "../profile/PulsarProfileManager.h"
#include <fstream>
#include "PulsarCore.h"

using namespace std;

void CommandHandler::execute(string command) {
    vector<string> command_split = split(command, " ");
    if (command_split[0] == "pulsar") {}
    else if (command_split[0] == "setrule") {
        CommandSetrule command_setrule;
        command_setrule.execute(command_split);
    }
    else if (command_split[0] == "config") {
        CommandConfig command_config;
        command_config.execute(command_split);
    }
}

void CommandSetrule::execute(const vector<string> &command) {
    if (command[1] == "showWarnings") {
        toml::table conf = toml::parse_file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\config.toml" );
        conf.insert_or_assign("showWarnings", command[2] == "true");
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