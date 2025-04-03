#include "wp.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <windows.h>
#include <fstream>
using namespace std;
string current_path;
string build_ID;

/*Коды возвращаемых ошибок
0001 - код выхода из системы*/

atomic<bool> timeThreadRunning(true);
mutex consoleMutex;

void PulsarConsoleClear();
int configAnalyze(string bildn, string AccountName);

class CurrentPulsarInfo {
public:
    static string title;
    static string platform_version;
    static string account;
    static int start_time;
    static string bildingid;

    static string getCurrentDateTime() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm local_time;
        localtime_s(&local_time, &now_time);

        stringstream datetime_stream;
        datetime_stream << put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        return datetime_stream.str();
    }

    static void ShowInfo() {
        unsigned int point_time = clock();
        cout << "----------------------------------------------" << endl;
        cout << "Версия:         " << title << endl;
        cout << "Платформа:      " << platform_version << endl;
        cout << "Время работы:   " << (point_time - start_time) / 1000 << " сек" << endl;
        cout << "Текущее время:  " << getCurrentDateTime() << endl;
        cout << "Аккаунт:        " << account << endl;
        cout << "ID сборки:      " << bildingid << endl;
        cout << "----------------------------------------------" << endl;
    }
};
string CurrentPulsarInfo::title = "PulsarVenv 0.0.1-alpha";
string CurrentPulsarInfo::platform_version = "Windows";
string CurrentPulsarInfo::account = "";
int CurrentPulsarInfo::start_time = 0;
string CurrentPulsarInfo::bildingid = "";

void puls_calc(string line) {
    line.replace(0, 4, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    string comforsys = "cd " + current_path + "\\\\SytemPuls\\systemmodules && pulsarcalc.exe " + line;
    system(comforsys.c_str());
}

void puls_python(string line) {
    if (build_ID != "0000") {
        cout << "В данной сборке отсутствует эта команда..." << endl;
    }
    else {
        line.replace(0, 7, "");
        size_t first_non_space = line.find_first_not_of(' ');
        if (first_non_space != string::npos) {
            line.erase(0, first_non_space);
        }
        string pfile = "cd " + current_path + "\\python313puls && ppython.exe " + line;
        system(pfile.c_str());
    }
}

void puls_sysconfig(string line) {
    line.replace(0, 9, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    string scfile = "cd " + current_path + "\\\\SytemPuls\\systemmodules && sysconfig.exe";
    cout << "Определение параметров системы... Пожалуйста подождите...\n" << endl;
    system(scfile.c_str());

}

void sumulator_pulsar() {
    string pathToSumPuls = "cd " + current_path + "\\SytemPuls\\systemmodules && pulsarem.exe";
    cout << "Внимание скоро начнется эмуляция пульсара.\nВо время эмуляции не нажимайте по консольному окну.\n"
        << "Если нажали нажмите enter чтобы продолжить эмуляцию" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    system("cls");
    system(pathToSumPuls.c_str());
    PulsarConsoleClear();
}

void show_help() {
    /*cout << "\nДоступные команды:" << endl;
    cout << "  calc <выражение>  - Калькулятор (например: calc 2+2)" << endl;
    cout << "  python <скрипт>   - Запуск Python скрипта (только для сборки 0000)" << endl;
    cout << "  sysconfig         - Показать конфигурацию системы" << endl;
    cout << "  pinfo             - Показать информацию о системе" << endl;
    cout << "  clear             - Очистить экран" << endl;
    cout << "  help              - Показать эту справку" << endl;
    cout << "  exit              - Выйти из PulsarVenv" << endl;
    cout << "  sumulator_pulsar  - Запустить эмуляцию жизни пульсара" << endl;
    cout << "  <имя_программы>   - Запуск программы из папки modules\n" << endl;*/
    string PathToOpen = "cd " + current_path + "\\public\\documentation && notepad PulsarCommandHelp.txt";
    system(PathToOpen.c_str());
}

void PulsarConsoleClear() {
    CurrentPulsarInfo clearinf;
    system("cls");
    cout << clearinf.title << endl;
}

void AccountCommand(string line) {
    line.replace(0, 8, "");
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);
    string name, password;
    if (line.starts_with("add")) {
        cout << "Введите имя аккаунта: ";
        getline(cin, name);
        cout << "СПРАВКА - если хотите создать аккаунт без пароля укажите \"none\"" << endl;
        while (true) {
            cout << "Введите пароль аккаунта: ";
            getline(cin, password);
            if (password.length() < 4) {
                cout << "\nПароль должен содержать минимум 4 символа";
                continue;
            }
            break;
        }
        string newDirecrotyAccount = current_path + "\\accounts\\" + name;
        filesystem::create_directory(newDirecrotyAccount);
        filesystem::create_directory(newDirecrotyAccount + "\\accountcfg");
        filesystem::create_directory(newDirecrotyAccount + "\\userfiles");
        ofstream f;
        f.open(newDirecrotyAccount + "\\accountcfg\\password.ppas");
        f << password;
        f.close();
        cout << "Успешно создан аккаунт\nИмя - " << name << "     Пароль - " << password << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1100));
    }
    else if (line.starts_with("remove")) {

        cout << "Введите имя аккаунта: ";
        getline(cin, name);
        string pathToAccount = current_path + "\\accounts\\" + name;
        cout << "Введите пароль аккаунта: ";
        getline(cin, password);
        if (filesystem::exists(pathToAccount)) {
            fstream f;
            f.open(current_path + "\\accounts\\" + name + "\\accountcfg\\password.ppas", fstream::in | fstream::out | fstream::app);
            string correctPassword;
            getline(f, correctPassword);
            f.close();
            if (password == correctPassword) {
                filesystem::remove_all(pathToAccount);
                cout << "Аккаунт успешно удален." << endl;
            }
            else {
                cout << "Неверный пароль" << endl;
            }

        }
        else {
            cout << "Такого аккаунта не существует" << endl;
        }
    }
    else if (line.starts_with("swap")) {
        cout << "Введите имя аккаунта: ";
        getline(cin, name);
        string pathToAccount = current_path + "\\accounts\\" + name;
        cout << "Введите пароль аккаунта: ";
        getline(cin, password);
        if (filesystem::exists(pathToAccount)) {
            fstream f;
            f.open(current_path + "\\accounts\\" + name + "\\accountcfg\\password.ppas", fstream::in | fstream::out | fstream::app);
            string correctPassword;
            getline(f, correctPassword);
            f.close();
            if (password == correctPassword) {
                configAnalyze(build_ID, name);
                PulsarConsoleClear();
            }
            else {
                cout << "Неверный пароль" << endl;
            }

        }
        else {
            cout << "Такого аккаунта не существует" << endl;
        }
    }


}

int configAnalyze(string bildn, string AccountName) {
    CurrentPulsarInfo::start_time = clock();
    CurrentPulsarInfo::bildingid = bildn;
    CurrentPulsarInfo::account = AccountName;

    string setConsoleTitlePuls = "title PulsarVenv " + AccountName;
    system(setConsoleTitlePuls.c_str());
    build_ID = bildn;
    filesystem::path tfp = filesystem::current_path();
    current_path = tfp.string();
    return 0;
}
int pulsarstart(string bildn, string AccountName) {
    setlocale(LC_ALL, "Ru");
    configAnalyze(bildn, AccountName);
    string com;
    system("cls");
    cout << CurrentPulsarInfo::title << endl; 

    while (true) {
        cout << "$> ";
        getline(cin, com);
        com.erase(0, com.find_first_not_of(' '));
        com.erase(com.find_last_not_of(' ') + 1);

        if (com.empty()) {
            continue;
        }
        else if (com == "exit") {
            return 0101;
        }
        else if (com.substr(0, 4) == "calc") {
            if (com.length() > 4) {
                puls_calc(com);
            }
            else {
                cout << "Ошибка: Введите выражение для вычисления (например: calc 2+2)" << endl;
            }
        }
        else if (com == "clear") {
            PulsarConsoleClear();
        }
        else if (com == "pinfo") {
            CurrentPulsarInfo::ShowInfo();
        }
        else if (com.substr(0, 7) == "python") {
            if (com.length() > 7) {
                puls_python(com);
            }
            else {
                cout << "Ошибка: Укажите скрипт для запуска (например: python script.py)" << endl;
            }
        }
        else if (com.substr(0, 9) == "sysconfig") {
            puls_sysconfig(com);
        }
        else if (com == "help" || com.substr(0, 4) == "help") {
            show_help();
        }
        else if (com == "sumulator_pulsar") {
            sumulator_pulsar();
        }
        else if (com.substr(0, 7) == "account") {
            AccountCommand(com);
        }
        else {
            string checkfile;
            if (com.find(".exe") != string::npos) {
                checkfile = current_path + "\\modules\\" + com;
            }
            else {
                checkfile = current_path + "\\modules\\" + com + ".exe";
            }

            if (filesystem::exists(checkfile)) {
                string cdmodule = "cd " + current_path + "\\modules && " + com;
                system(cdmodule.c_str());
            }
            else {
                cout << "Ошибка: Команда не распознана. Введите 'help' для справки." << endl;
            }
        }
    }
}