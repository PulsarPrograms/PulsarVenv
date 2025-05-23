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
    CommandLog::write_in_log(command, false,false);
    string command_without_quotes = remove_quotes(command);
    for (auto&& [alias_key_node, alias_value] : PulsarCore::alias) {
        std::string alias_key(alias_key_node.data(), alias_key_node.length());

        if (alias_value.is_string()) {
            std::string alias_val = alias_value.as_string()->get();

            if (command_without_quotes == alias_key) {
                command_without_quotes = alias_val;
            }
        }
    }

    vector<string> command_split = split(command_without_quotes);
        for (size_t i = 0; i < command_split.size(); i++) {
            if (command_split[i].rfind("//", 0) == 0) {
                command_split.erase(command_split.begin() + i);
                i--;
            }
        }
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
        else if (command_split[0] == "alias") {
            CommandAlias command_alias;
            command_alias.execute(command_split);
        }
        else if (command_split[0] == "log") {
            CommandLog command_log;
            command_log.execute(command_split);
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
        else if (command_split[0] == "script") {
            CommandScript command_script;
            command_script.execute(command_split);
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
        else {
            cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
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

int CommandScript::execute(const vector<string>& command) {
    if (command[1] == "start") {
        if (command.size() >= 3) {
            if (command[2] == "-a") {
                if (command.size() >= 4) {
                    string line;
                    fstream file(command[3], fstream::in | fstream::out | ios::app);
                    if (!(file.is_open())) {
                        cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                        return 1;
                    }
                    while (getline(file,line)) {
                        CommandHandler::execute(line);
                    }
                    file.close();
                }
                else {
                    cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                }
            }
            else {

                fstream file(PulsarCore::current_path + "/system/scripts/" + command[2],fstream::in | fstream::out | ios::app );
                if (!(file.is_open())) {
                    cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                    return 1;
                }
                string line;
                while (getline(file,line)) {
                    CommandHandler::execute(line);
                }
                file.close();
            }
        }
        else {
            cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
        }
        return 0;
    }
}


int CommandAlias::execute(const std::vector<std::string> &command) {
    if (command[1] == "create") {
        PulsarCore::alias.insert_or_assign(command[2], command[3]);
        ofstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\alias.toml");
        if (!(file.is_open())) {
            cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
        }
        file << PulsarCore::alias;
        file.close();
    }
    else if (command[1] == "remove") {
        if (PulsarCore::alias.contains(command[2])) {
            PulsarCore::alias.erase(command[2]);
            ofstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\alias.toml");
            if (!(file.is_open())) {
                cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
            }
            file << PulsarCore::alias;
            file.close();
        }
    }
    else if (command[1] == "show") {
        fstream file(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\alias.toml");
        if (!(file.is_open())) {
            cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
        }
        string line;
        while (getline(file,line)) {
            cout << line << endl;
        }
        file.close();

    }
    return 0;
}

int CommandLog::write_in_log(const std::string text, bool is_time, bool is_trunc) {
    auto mode = (is_trunc) ? ios::trunc : ios::app;
    fstream log(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\log.pulslog", fstream::in | fstream::out |mode );
    log << text << ((is_time)? " " + get_current_date_time() : "");
    log << "\n";
    log.close();
    return 0;
}


int CommandLog::execute(const std::vector<std::string> &command) {
    if (command.size()>=4 && command[1] == "write") {
        CommandLog::write_in_log(command[2], command[3] == "true");
    }
    else if (command.size()>=2 && command[1] == "show") {
        string line;
        fstream log(PulsarCore::current_path + "\\system\\profiles\\" + PulsarCurrentProfile::name + "\\settings\\log.pulslog", fstream::in | fstream::out |  ios::app);
        while (getline(log,line)) {
            cout << line << endl;
        }
        log.close();
    }
    else if (command.size()>=2 && command[1] == "clear") {
        if (PulsarCurrentProfile::showWarnings) {
            string verify;
            cout << PulsarCore::pulsar_locale["danger_com"].value_or("ERROR: [LOCALE ERROR]") <<endl;
            cout << PulsarCore::pulsar_locale["danger_step"].value_or("ERROR: [LOCALE ERROR]") << " 1" << endl;
            cout << PulsarCore::pulsar_locale["op_verify"].value_or("ERROR: [LOCALE ERROR]") << " ";  getline(cin, verify);
            if (verify == "Y" || verify == "y") {
                CommandLog::write_in_log(" %% pulsar log clear", true, true);
            }
            else {
                cout << PulsarCore::pulsar_locale["op_stop"].value_or("ERROR: [LOCALE ERROR]") << " " << PulsarCore::pulsar_locale["user_not_agree"].value_or("ERROR: [LOCALE ERROR]") << endl;
            }
        }
        else {
            CommandLog::write_in_log(" %% pulsar log clear", true, true);
        }

    }
    return 0;
}
