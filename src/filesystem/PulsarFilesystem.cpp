
#include "PulsarFilesystem.h"

#include <filesystem>
#include <string>
#include <vector>
#include "../utils/utils.h"
#include "../core/PulsarCore.h"
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

fs::path PulsarFilesystem::cur_path = fs::current_path() / "system" / "pulsfs" / "pulsarvenv" / "home";
fs::path PulsarFilesystem::home_path = fs::current_path() / "system" / "pulsfs" / "pulsarvenv" / "home";
fs::path PulsarFilesystem::last_path = fs::current_path() / "system" / "pulsfs" / "pulsarvenv" / "home";


void PulsarFilesystem::handle_cd(const std::string path) {
    if (path == "..") {
        PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
        PulsarFilesystem::cur_path = cur_path.parent_path();
    }
    else if (path == "....") {
        PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
        PulsarFilesystem::cur_path = cur_path.parent_path().parent_path();
    }
    else if (path == "~LAST~"){
        PulsarFilesystem::cur_path = PulsarFilesystem::last_path;
    }
    else if (path == "~"){
        PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
        PulsarFilesystem::cur_path = PulsarFilesystem::home_path;
    }
    else {
        fs::path target = cur_path / path;
    
        if (fs::exists(target) && fs::is_directory(target)) {
            PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
            PulsarFilesystem::cur_path = target;
        }
        else if (fs::exists(path) && fs::is_directory(path)) {
            PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
            PulsarFilesystem::cur_path = path;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["invalid_path"].value_or("Locale ERROR"));
        }

    }
}

void PulsarFilesystem::handle_mkdir(const std::string path) {
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        
        target = cur_path / path;
    }

    
    if (fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["path_ex"].value_or("Locale ERROR"));
    }
    else {
        fs::create_directories(target);
    }
}

void PulsarFilesystem::handle_mkсd(const std::string path){
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        target = cur_path / path;
    }

    if (fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["path_ex"].value_or("Locale ERROR"));
    }
    else {
        fs::create_directories(target);
        PulsarFilesystem::last_path = PulsarFilesystem::cur_path;
        PulsarFilesystem::cur_path = target;
    }
}

void PulsarFilesystem::handle_rmdir(const std::string path){
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        target = cur_path / path;
    }


    if (!fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["dir_not_ex"].value_or("Locale error"));

        return;
    }

    if (!fs::is_directory(target)) {
        cout_err(PulsarCore::pulsar_locale["path_not_dir"].value_or("Locale error"));
        return;
    }

    std::error_code ec;
        uintmax_t n = fs::remove_all(target, ec);
        if (ec) {
            cout_err(PulsarCore::pulsar_locale["remdir_er"].value_or<std::string>("Locale error") + ec.message());
        } else {
            cout << PulsarCore::pulsar_locale["rem"].value_or<std::string>("Locale error") << " " << n << " " << PulsarCore::pulsar_locale["file_dir"].value_or<std::string>("Locale error") << endl;
        }

}

void PulsarFilesystem::handle_ls(const std::filesystem::path path) {

    fs::path target = path.is_absolute() ? path : (cur_path / path);


    if (fs::exists(target) && fs::is_directory(target)) {
        for (const auto &entry : fs::directory_iterator(target)) {
            if (entry.is_directory()){
            set_color(11); cout << " | " << entry.path().filename().string() << "\n"; set_color(7);
            }
            else {
                cout << " | " << entry.path().filename().string() << "\n";
            }
            
        }
    }
    else {
        cout_err(PulsarCore::pulsar_locale["path_not_dir"].value_or("Locale error"));
    }
}


void PulsarFilesystem::handle_crfile(const std::filesystem::path path) {
    
    fs::path target = path.is_absolute() ? path : (cur_path / path);

    if (fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["file_is_ex"].value_or<string>("Locale error") + target.string());
        return;
    }

    if (!target.parent_path().empty()) {
        fs::create_directories(target.parent_path());
    }

    std::ofstream file(target);
    if (!file.is_open()) {
        cout_err(PulsarCore::pulsar_locale["crfile_er"].value_or<string>("Locale error") + target.string());
        return;
    }
    file.close();
    
}

void PulsarFilesystem::handle_morefile(const string path) {
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        target = cur_path / path;
    }

    if (!fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["file_not_ex"].value_or<string>("Locale error") + target.string());
        return;
    }
    if (fs::is_directory(target)){
        cout_err(PulsarCore::pulsar_locale["path_is_dir"].value_or<string>("Locale error") + target.string());
        return;
    }

    std::ifstream file(target);
    if (!file.is_open()) {
        cout_err(PulsarCore::pulsar_locale["morefile_er"].value_or<string>("Locale error") + target.string());
        return;
    }
    string line;
    while (getline(file, line)){
        cout <<  line << endl;
    }
    file.close();
}

void PulsarFilesystem::handle_writefile(const string path, const string text){
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        target = cur_path / path;
    }

    if (!fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["file_not_ex"].value_or<string>("Locale error") + target.string());
        return;
    }
    if (fs::is_directory(target)){
        cout_err(PulsarCore::pulsar_locale["path_is_dir"].value_or<string>("Locale error") + target.string());
        return;
    }

    std::ofstream file(target, ios::app);
    if (!file.is_open()) {
        cout_err(PulsarCore::pulsar_locale["morefile_er"].value_or<string>("Locale error") + target.string());
        return;
    }

    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == '\\' && i + 1 < text.length()) {
            switch (text[i+1]) {
                case 'n': file << '\n'; break;
                case 't': file << '\t'; break;
                default: file << text[i] << text[i+1]; break;
            }
            i++; 
        } else {
            file << text[i];
        }
    }
    file.close();
}

void PulsarFilesystem::handle_crwfile(const fs::path path, const string text){
    
    fs::path target = path.is_absolute() ? path : (cur_path / path);

    if (fs::exists(target)) {
        cout_err(PulsarCore::pulsar_locale["file_is_ex"].value_or<string>("Locale error") + target.string());
        return;
    }

    if (!target.parent_path().empty()) {
        fs::create_directories(target.parent_path());
    }

    std::ofstream file(target, ios::app);
    if (!file.is_open()) {
        cout_err(PulsarCore::pulsar_locale["crfile_er"].value_or<string>("Locale error") + target.string());
        return;
    }
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == '\\' && i + 1 < text.length()) {
            switch (text[i+1]) {
                case 'n': file << '\n'; break;
                case 't': file << '\t'; break;
                default: file << text[i] << text[i+1]; break;
            }
            i++; 
        } else {
            file << text[i];
        }
    }
    file.close();
}

void PulsarFilesystem::handle_rm(fs::path path){
    fs::path target = path.is_absolute() ? path : (cur_path / path);

    if (!(fs::exists(target))) {
        cout_err(PulsarCore::pulsar_locale["file_not_ex"].value_or<string>("Locale error") + path.string());
        return;
    }

    if (fs::remove(target)){
        cout_good(PulsarCore::pulsar_locale["file_del_true"].value_or<string>("Locale error") + path.string());
    }
    else {
        cout_good(PulsarCore::pulsar_locale["file_del_false"].value_or<string>("Locale error") + path.string());
    }
}

void PulsarFilesystem::handle_cd(){
    if (fs::exists(PulsarFilesystem::home_path) && fs::is_directory(PulsarFilesystem::home_path)) {
        PulsarFilesystem::cur_path = PulsarFilesystem::home_path;
    }
}

void PulsarFilesystem::handle_tree() {
    for (const auto &entry : fs::directory_iterator(PulsarFilesystem::cur_path)) {
        if (entry.is_directory()){
            set_color(11); cout << " | " << entry.path().filename().string() << "\n"; set_color(7);
        fs::path p = entry;
        PulsarFilesystem::handle_sys_tree(p);
        }
        else { 
            cout << " | " << entry.path().filename().string() << "\n";
            
        }     
    }
    return;
}


void PulsarFilesystem::handle_sys_tree(fs::path path) {
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_directory()){
            set_color(11); cout << "   <|  " << entry.path().filename().string() << "\n"; set_color(7);
        fs::path p = entry;
        PulsarFilesystem::handle_sys_tree(p);
        }
        else { 
            cout << " | " << entry.path().filename().string() << "\n";
            
        }     
    }
    return;
}


void PulsarFilesystem::handle_fsdown(int pos){
    int cur_pos = 1;
    for (const auto &entry : fs::directory_iterator(PulsarFilesystem::cur_path)) {
        if (entry.is_directory()){
            if (cur_pos == pos){
            PulsarFilesystem::cur_path = entry;
            break;
            }
            cur_pos++;
        }
        
    }
}

void PulsarFilesystem::handle_betcd(const string& fdir) {
    try {
        if (find_and_change_dir(cur_path, fdir)) {
            return; 
        }
    } 
    catch (const fs::filesystem_error& e) {
        cout_err("Filesystem error: " + string(e.what()));
    }
}

bool PulsarFilesystem::find_and_change_dir(const fs::path& search_path, const string& target_dir) {
    try {
        for (const auto& entry : fs::directory_iterator(search_path)) {
            if (entry.is_directory()) {
                if (entry.path().filename() == target_dir) {
                    last_path = cur_path;
                    cur_path = entry.path();
                    return true;
                }
                
                if (find_and_change_dir(entry.path(), target_dir)) {
                    return true;
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        return false;
    }
    
    return false;
}


int CommandFilesystem::execute(std::vector<std::string> &command) {
    if (command.size() == 2 && command[0] == "cd") {
        PulsarFilesystem::handle_cd(command[1]);
    }
    else if (command.size() == 1 && command[0] == "cd"){
        PulsarFilesystem::handle_cd();
    }
    else if (command.size() == 2 && command[0] == "betcd"){
        PulsarFilesystem::handle_betcd(command[1]);
    }
    else if (command.size() == 1 && command[0] == "pwd") {
        cout << PulsarFilesystem::cur_path.string() << endl;
    } 
    else if (command.size() == 2 && command[0] == "mkdir"){
        PulsarFilesystem::handle_mkdir(command[1]);
    }
    else if (command.size() == 2 && command[0] == "mkcd"){
        PulsarFilesystem::handle_mkсd(command[1]);
    }
    else if (command.size() == 2 && command[0] == "rmdir"){
        PulsarFilesystem::handle_rmdir(command[1]);
    }
    else if (command.size() == 1 && command[0] == "ls"){
        PulsarFilesystem::handle_ls();
    } 
    else if (command.size() == 2 && command[0] == "ls"){
        fs::path path = command[1];
        PulsarFilesystem::handle_ls(path);
    } 
    else if (command.size() == 2 && command[0] == "crfile"){
        fs::path path = command[1];
        PulsarFilesystem::handle_crfile(path);
    }
    else if (command.size() == 2 && command[0] == "more"){
        PulsarFilesystem::handle_morefile(command[1]);
    }
    else if (command.size() == 3 && command[0] == "writef"){
        PulsarFilesystem::handle_writefile(command[1],command[2]);
    }
    else if (command.size() == 3 && command[0] == "crwfile") {
        fs::path path = command[1];
        PulsarFilesystem::handle_crwfile(path, command[2]);
    }
    else if (command.size() == 2 && command[0] == "rm") {
        fs::path path = command[1];
        PulsarFilesystem::handle_rm(path);
    }
    else if (command.size() == 1 && command[0] == "fsdown"){
        PulsarFilesystem::handle_fsdown();
    }
    else if (command.size() == 2 && command[0] == "fsdown"){
        int posit;
        try {
        posit = stoi(command[1]);
        } catch (...){
            cout_err(PulsarCore::pulsar_locale["invalid_stoi"].value_or("Locale error"));
            return 1;
        }
        PulsarFilesystem::handle_fsdown(posit);
    }
    else if (command.size() == 1 && command[0] == "tree"){
        PulsarFilesystem::handle_tree();
    }
    else {
        return 1;
    }
    return 0;
}
