#include "CommandHandler.h"

#include <iostream>
#include <../utils/utils.h>
#include <string>
#include <toml++/toml.h>
#include "../profile/PulsarProfileManager.h"
#include <fstream>
#include "PulsarCore.h"
#include "../filesystem/PulsarFilesystem.h"

using namespace std;

int CommandHandler::execute(string command) {
    CommandLog::write_in_log(command, false,false); // Запись в лог
    string command_without_quotes = remove_quotes(command); // удаление начальных и конечных кавычек
    command_without_quotes = strip(command_without_quotes); // Удаление пробелов в начале и конце

    // Начало разбора алиасов 
    for (auto&& [alias_key_node, alias_value] : PulsarCore::alias) {
        std::string alias_key(alias_key_node.data(), alias_key_node.length());

        if (alias_value.is_string()) {
            std::string alias_val = alias_value.as_string()->get();

            if (command_without_quotes == alias_key) {
                command_without_quotes = alias_val;
            }
        }
    }

    // Конец разбора алиасов

    vector<string> command_split = split(command_without_quotes); // Разбитие по пробелу, игнорирую пробелы в кавыках


    // Поиск и удаление комментариев
    for (size_t i = 0; i < command_split.size(); i++) {
        if (command_split[i].rfind("//", 0) == 0) {
            command_split.erase(command_split.begin() + i, command_split.end());
            break;
        }
    }

    //команды из разбитого массива
    if (command_split.size() >= 1 ) {
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
            // проверка включены ли бета функции
            if (PulsarCurrentProfile::betaFunc) {
                CommandAlias command_alias;
                command_alias.execute(command_split);
            }
            else {
                cout_err(PulsarCore::pulsar_locale["try_beta_func"].value_or("ERROR: [LOCALE ERROR] "));
                return 111;
            }
        }
        else if (command_split[0] == "log") {
            CommandLog command_log;
            command_log.execute(command_split);
        }

        else if (command_split[0] == "calc") { 
            // вызов калькулятора
            string exten = (PulsarCore::platform == "Windows") ? ".exe" : "";
            string calc_arg = (command_split.size() >= 2) ? command_split[1] : "";
            string starter = (PulsarCore::platform == "Windows") ? "pulsarcalc" : "./pulsarcalc";
            string commandSys ="cd " + PulsarCore::current_path + "/system/systemmodules && " + starter + exten + " " + calc_arg;
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
        }        else if (command_split[0] == "out") {
            if (command.size() >= 2) {
                cout << command_split[1] << endl;
                cout << "\n";
            }
            else {
                cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<std::string>("ERROR: [LOCALE ERROR] ") + " out" );
            }

        }
        else {
            CommandUtils cu;
            CommandFilesystem cf;
            if (cu.execute(command_split) != 0) {
                if (cf.execute(command_split) != 0) {
                    cout_err(PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] "));
                }
            }

        }
    }
    else {

            string buffer;
            for (const string &com : command_split) {
                buffer += com;
            }
            if (buffer.empty()) {
                return 0;
            }
            cout_err(PulsarCore::pulsar_locale["invalid_value"].value_or<string>("ERROR: [LOCALE ERROR] ") + " " + buffer);

    }
    return 0;
}

void CommandSetrule::execute(const vector<string> &command) {
    if (command.size() == 2) {
        if (command[1] == "list") {
            for (const string &c : com) {
                cout << c << ", ";
            }
            cout << endl;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "setrule " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else if (command.size() == 3 && command[1] != "list") {
        if (command[1] == "showWarnings" && command.size() == 3) {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml" );
            if (!(command[2] == "true" || command[2] == "false")) {
                cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                return;
            }
            conf.insert_or_assign("showWarnings", command[2] == "true");
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update();
        }
        else if (command[1] == "setLocale" && command.size() == 3) {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml" );
            conf.insert_or_assign("locale", command[2]);
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update();
        }
        else if (command[1] == "setTheme"&& command.size() == 3) {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml" );
            conf.insert_or_assign("theme", command[2]);
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update(true);
        }
        else if (command[1] == "betaFunc"&& command.size() == 3) {
            toml::table conf = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml" );
            if (!(command[2] == "true" || command[2] == "false")) {
                cerr << PulsarCore::pulsar_locale["invalid_value"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                return;
            }
            conf.insert_or_assign("betaFunc", command[2] == "true");
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update();
        }
        else if (command[1] =="themeColor" && command.size() == 3) {
            string color = command[2];
            int icolor = 7;

            if (color == "red") icolor = 12;
            else if (color == "green") icolor = 10;
            else if (color == "yellow") icolor = 14;
            else if (color == "blue") icolor = 9;
            else if (color == "magenta") icolor = 13;
            else if (color == "white") icolor = 15;
            else if (color == "cyan") icolor = 11;
            else if (color == "grey") icolor = 8;
            else if (color == "reset") icolor = 7;
            else if (color == "purple") icolor = 5;
            else if (color == "pink") icolor = 13;
            else if (color == "neon-cyan") icolor = 11;

            toml::table conf = toml::parse_file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml" );
            conf.insert_or_assign("themeColor", icolor);
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            file << conf;
            file.close();
            PulsarCore::account_update(true);
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "setrule " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else {
        cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<string>("Locale error") + " setrule");
    }
}


void CommandConfig::execute(const vector<string> &command) {
    if (command.size() == 2) {
        if (command[1] == "show") {
            string show_line;
            ifstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/config.toml");
            while (getline(file,show_line)) {
                cout << show_line << endl;
            }
        }
        else if (command[1] == "list") {
            for (const string &com : command) {
                cout << com << ", ";
            }
            cout <<endl;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "config " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else {
        cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<string>("Locale error") + " config");
    }
}

void CommandPulsar::execute(const vector<string>& command) {
    if (command.size() == 2) {
        if (command[1] == "info") {
            PulsarCurrentProfile::show_info();
        }
        else if (command[1] == "list") {
            for (const string &c : com) {
                cout << c << ", ";
            }
            cout << endl;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "pulsar " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else {
        cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<string>("Locale error") + " pulsar");
    }
}

int CommandScript::execute(const vector<string>& command) {
    if (command.size() == 2) {
        if (command[1] == "list") {
            for (const string &c : com) {
                cout << c << ", ";
            }
            cout << endl;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "script " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }

    }
    else if (command.size() == 3 && command[1] != "list" && !(filesystem::path(command[2]).is_absolute())) {
        if (command[1] == "start") {
            filesystem::path target = command[2];
            if (filesystem::exists(PulsarFilesystem::cur_path / target)){
                ifstream file(PulsarFilesystem::cur_path / target);
                if (!(file.is_open())) {
                    cout_err(PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] "));
                    return 1;
                }
                string line;
                while (getline(file,line)) {
                    CommandHandler::execute(line);
                }
                file.close();
            }
            else if (filesystem::exists(PulsarCore::current_path + "/system/scripts/" + command[2])){
                ifstream file(PulsarCore::current_path + "/system/scripts/" + command[2]);
                if (!(file.is_open())) {
                    cout_err(PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] "));
                    return 1;
                }
                string line;
                while (getline(file,line)) {
                    CommandHandler::execute(line);
                }
                file.close();
            }
            else {
                cout_err(PulsarCore::pulsar_locale["script_not_ex"].value_or("ERROR: [LOCALE ERROR] "));
            }
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "script " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else if (command.size() == 3 && command[1] != "list" && filesystem::path(command[2]).is_absolute()) {
        if (command[1] == "start") {
                string line;
                ifstream file(command[2]);
                if (!(file.is_open())) {
                    cout_err(PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] "));
                    return 1;
                }
                while (getline(file,line)) {
                    CommandHandler::execute(line);
                }
                file.close();
            
            

        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "script " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else {
        cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<string>("ERROR: [LOCALE ERROR] ") + "script ");
    }
    return 0;
}




int CommandAlias::execute(const std::vector<std::string> &command) {
    if (command.size() == 2) {
        if (command[1] == "show") {
            fstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/alias.toml");
            if (!(file.is_open())) {
                cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
            }
            string line;
            while (getline(file,line)) {
                cout << line << endl;
            }
            file.close();
        }
        else if (command[1] == "list") {
            for (const string &c : com) {
                cout << c << ", ";
            }
            cout << endl;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "alias " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    }
    else if (command.size() == 3 && command[1] != "show"  && command[1] != "list") {
         if (command[1] == "remove") {
            if (PulsarCore::alias.contains(command[2])) {
                PulsarCore::alias.erase(command[2]);
                ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/alias.toml");
                if (!(file.is_open())) {
                    cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
                }
                file << PulsarCore::alias;
                file.close();
            }
        }
         else {
             cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "alias " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
         }
    }
    else if (command.size() == 4 && command[1] != "show"  && command[1] != "list" && command[1] != "remove") {
        if (command[1] == "create") {
            PulsarCore::alias.insert_or_assign(command[2], command[3]);
            ofstream file(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/alias.toml");
            if (!(file.is_open())) {
                cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR]") <<endl;
            }
            file << PulsarCore::alias;
            file.close();
        }
        else {
            cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "alias " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + command[1]);
        }
    } else {
        cout_err(PulsarCore::pulsar_locale["invalid_arg"].value_or<string>("ERROR: [LOCALE ERROR] ") + "script ");
    }

    return 0;
}

int CommandLog::write_in_log(const std::string text, bool is_time, bool is_trunc) {
    auto mode = (is_trunc) ? ios::trunc : ios::app;
    fstream log(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/log.pulslog", fstream::in | fstream::out |mode );
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
        fstream log(PulsarCore::current_path + "/system/profiles/" + PulsarCurrentProfile::name + "/settings/log.pulslog", fstream::in | fstream::out |  ios::app);
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
    else {
        cout_err(PulsarCore::pulsar_locale["class_com"].value_or<string>("Locale error") + "log " + PulsarCore::pulsar_locale["not_have_com"].value_or<string>("Locale error") + ((command.size() >= 1) ? command[1] : ""));
    }
    return 0;
}
int CommandUtils::start_point_time = 0;
int CommandUtils::end_point_time = 0;
int CommandUtils::last_total_time = 0;

int CommandUtils::execute(const std::vector<std::string> &command) {
    if (command.size() >= 2 && command[0] == "timer") {
        if (command[1] == "start") {
            CommandUtils::start_point_time = clock();
        }
        else if (command[1] == "stop") {
            CommandUtils::end_point_time = clock();
            CommandUtils::last_total_time = (CommandUtils::end_point_time - CommandUtils::start_point_time) / 1000;
        }
        else if (command[1] == "show") {
            cout << CommandUtils::last_total_time << " sec." <<endl;
        }
    }
    else if (command.size() >= 2 && command[0] == "script_to_cmd") {
        if (command.size() >= 3 && command[1] == "-a") {
            string line;
            string buffer = "cd " + PulsarCore::current_path + " && PulsarVenv -l " + PulsarCurrentProfile::name + " -c ";
            fstream file(command[3], fstream::in | fstream::out | ios::app );
            if (!file.is_open()) {
                cerr << PulsarCore::pulsar_locale["file_error"].value_or("LOCALE ERROR") << command[3] << endl;
                return 1;
            }
            while (getline(file,line)) {
                buffer += "\"" + line + "\" ";
            }
            file.close();
            cout << buffer << endl;
        }
        else {

            fstream file(PulsarCore::current_path + "/system/scripts/" + command[2],fstream::in | fstream::out | ios::app );
            if (!(file.is_open())) {
                cerr << PulsarCore::pulsar_locale["file_error"].value_or("ERROR: [LOCALE ERROR] ") << endl;
                return 1;
            }
            string line;
            string buffer = "cd " + PulsarCore::current_path + " && PulsarVenv -l " + PulsarCurrentProfile::name + " -c ";
            while (getline(file,line)) {
                buffer += "\"" + line + "\" ";
            }
            file.close();
            cout << buffer << endl;
        }
    }
    else {
        return 1;
    }
    return 0;
}

