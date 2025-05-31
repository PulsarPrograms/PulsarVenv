/*
! Пересмотреть работу большинства функций файла
? Узнать почему иногда текущий путь не является дирректорией
TODO 52 строчка
* Editor 31/05/2025 - @zeroqxq
* Exit time : 31/05/2025 18:02
*/

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

void PulsarFilesystem::handle_cd(const std::string path) {
    if (path == "..") {
        PulsarFilesystem::cur_path = cur_path.parent_path();
    }
    else if (path == "....") {
        PulsarFilesystem::cur_path = cur_path.parent_path().parent_path();
    }
    else {
        fs::path target = cur_path / path;
    
        if (fs::exists(target) && fs::is_directory(target)) {
            PulsarFilesystem::cur_path = target;
        }
        else if (fs::exists(path) && fs::is_directory(path)) {
            PulsarFilesystem::cur_path = path;
        }
        else {
            cout_err(PulsarCore::pulsar_locale["invalid_path"].value_or("Locale ERROR"));
        }

    }
}

void PulsarFilesystem::handle_mkdir(const std::string path) {
    // ! эта функция почему то создает в корне проекта (это было потому что не укзаан папку сустем)
    // * прошлый комментарий уже исправлен
    fs::path target;

    if (fs::path(path).is_absolute()) {
        target = path;
    } else {
        // TODO : написать cout для просмотра чему равняется текущйи путь
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

void PulsarFilesystem::handle_ls() {
    cout << "Current path: " << PulsarFilesystem::cur_path << endl;
    cout << "Exists: " << fs::exists(PulsarFilesystem::cur_path) << endl;
    cout << "Is directory: " << fs::is_directory(PulsarFilesystem::cur_path) << endl;

    if (fs::exists(cur_path) && fs::is_directory(cur_path)) {
        for (const auto &entry : fs::directory_iterator(PulsarFilesystem::cur_path)) {
            cout << "  | " << entry.path().filename().string() << "\n";
        }
        cout << endl;
    }
    else {
        cout_err(PulsarCore::pulsar_locale["path_not_dir"].value_or("Locale error"));
    }
}


int CommandFilesystem::execute(std::vector<std::string> &command) {
    if (command.size() == 2 && command[0] == "cd") {
        PulsarFilesystem::handle_cd(command[1]);
    }
    else if (command.size() == 1 && command[0] == "pwd") {
        cout << PulsarFilesystem::cur_path.string() << endl;
    } else if (command.size() == 2 && command[0] == "mkdir"){
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
    else {
        return 1;
    }
    return 0;
}
