// Неообходимые библиотеки/модули
#include "C:\Users\user\Desktop\PulsarVenv Main\public\source\WindowsPulsar\winPulsar.h"
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
#include <cctype>
#include "C:\Users\user\source\repos\PulsarVenv\PulsarVenv\pulsFileSystem.h"

// Макросы цветов
#define CLEAR "\x1b[0m"
#define RED "\x1b[31m"	
#define GREEN "\x1b[32m"
#define BOLD "\x1b[1m"

//Пространство имен std
using namespace std;


// Глобальные переменные
string current_path;
string build_ID;
string style = "";
bool isLog = true;
atomic<bool> timeThreadRunning(true);
mutex consoleMutex;

// Скелеты (прототипы) функций
int PulsarConsoleClear();
int configAnalyze(string bildn, string AccountName, string pas);
bool isValidHexColor(string s);
int changeStyle();
bool isValidHexDigit(char c);
void puls_calc(string line);
void puls_sysconfig(string line);
void show_help();
int PulsarConsoleClear();
int neuro();
int AccountCommand(string line);
int configAnalyze(string bildn, string AccountName, string pas);
int comAnalyze(string line);
int executePulsarScript(string line);
int pulsStarterScript();
int writeInLog(string message, bool isTime, bool isTrunc);

// Главный класс информации о пульсаре
class CurrentPulsarInfo {
public:
    static string title;
    static string platform_version;
    static string account;
    static string password;
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
        cout << "Пароль:         " << password << endl;
        cout << "ID сборки:      " << bildingid << endl;
        cout << "----------------------------------------------" << endl;
    }
};

// Реализация статический переменных class CurrentPulsarInfo
string CurrentPulsarInfo::title = "PulsarVenv 0.3.0";
string CurrentPulsarInfo::platform_version = "Windows";
string CurrentPulsarInfo::account = "";
int CurrentPulsarInfo::start_time = 0;
string CurrentPulsarInfo::bildingid = "";
string CurrentPulsarInfo::password = "";


//Основная функция. Здесь начинается жизнь пульсара
int pulsarStart(string bildn, string AccountName, string pas) {
    setlocale(LC_ALL, "Ru");
    ofstream logfile;
    CurrentPulsarInfo::start_time = clock();
    configAnalyze(bildn, AccountName, pas);
    writeInLog("$startpulsar", true, false);
    string com;
    int code;
    system("cls");
    PulsarConsoleClear();
    pulsStarterScript();
    while (true) {
        cout << "$> ";
        getline(cin, com);
        com.erase(0, com.find_first_not_of(' '));
        com.erase(com.find_last_not_of(' ') + 1);
        if (com.starts_with("script")) {
            com.replace(0, 6, "");
            com.erase(0, com.find_first_not_of(' '));
            com.erase(com.find_last_not_of(' ') + 1);
            code = executePulsarScript(com);
        }
        else {
            code = comAnalyze(com);
        }
        if (code == 0101) {
            writeInLog("$exitpulsar", true, false);
            return 0101; 
        }
       
    }
}


// Реализация всех функций

int writeInLog(string message, bool isTime, bool isTrunc){
    ofstream logfile;
    logfile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\log.plog", fstream::in | fstream::out | ((isTrunc) ? ios::trunc : ios::app));
    logfile << message;
    logfile << ((isTime) ? " " + CurrentPulsarInfo::getCurrentDateTime() : "");
    logfile << "\n";
    logfile.close();
}

int changeStyle() {
    string newColor, newStyle;
    string name = CurrentPulsarInfo::account;
    fstream f;
    f.open(current_path + "\\accounts\\" + name + "\\accountcfg\\style.pcfg", fstream::in | fstream::out | ios::trunc);
    string correctPassword;
    while (1) {
        cout << "Введите цвет интерфеса 2 - шестнандцатиричных числа: (help для помощи): ";
        getline(cin, newColor);
        if (newColor == "help") {
            cout << "Атрибуты цветов задаются в виде ДВУХ шестнадцатеричных цифр — первая" << endl;
            cout << "задает цвет фона, а вторая определяет цвет переднего плана.Каждая цифра" << endl;
            cout << "может иметь следующие значения : " << endl;
            cout << "0 = Черный 8 = Серый" << endl;
            cout << "1 = Синий 9 = Светло - синий" << endl;
            cout << "2 = Зеленый A = Светло - зеленый" << endl;
            cout << "3 = Голубой B = Светло - голубой" << endl;
            cout << "4 = Красный C = Светло - красный" << endl;
            cout << "5 = Лиловый D = Светло - лиловый" << endl;
            cout << "6 = Желтый E = Светло - желтый" << endl;
            cout << "7 = Белый F = Ярко - белый" << endl;
        }
        else {
            if (!isValidHexColor(newColor)) {
                cout << "Неправильный ввод" << endl;
                continue;
            }
            f << newColor + "\n";
            break;
        }
    }
    while (true) {
        cout << "Введите стиль интерфейса : ";
        getline(cin, newStyle);
        f << newStyle + "\n";
        break;
    }
    f.close();
    configAnalyze(build_ID, CurrentPulsarInfo::account, CurrentPulsarInfo::password);
    PulsarConsoleClear();


}

bool isValidHexDigit(char c) {
    return isxdigit(c) != 0;
}

bool isValidHexColor(string s) {
    if (s.length() != 2) return false;
    return isValidHexDigit(s[0]) && isValidHexDigit(s[1]);
}

void puls_calc(string line) {
    line.replace(0, 4, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    string comforsys = "cd " + current_path + "\\\\SystemPuls\\systemmodules && pulsarcalc.exe " + line;
    system(comforsys.c_str());
}

void puls_sysconfig(string line) {
    line.replace(0, 9, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    cout << "Определение параметров системы... Пожалуйста подождите...\n" << endl;
    string PathwattSysInfo = "cd " + current_path + "\\SystemPuls\\" + "\\systemmodules\\" + "\\watt & watt.exe \"" + current_path + "\\SystemPuls\\" + "\\systemmodules\\" + "\\watt\\systeminfo\\sysinfo.wt\"";
    system(PathwattSysInfo.c_str());

}

void show_help() {
    cout << "Справочник по командам - https://github.com/PulsarPrograms/PulsarVenv/wiki" << endl << endl;
}

int PulsarConsoleClear() {
    system("cls");
    if (filesystem::exists(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.txt")) {
        fstream f;
        f.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.txt", fstream::in | fstream::out | ios::app);
        if (!f.is_open()) {
            cout << "Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.txt" << endl;
            return 1;
        }
        string linef;
        while (getline(f, linef)) {
            cout << linef << endl;
        }
    }
    else if (filesystem::exists(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.puls")) {
        fstream f;
        f.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.puls", fstream::in | fstream::out | ios::app);
        if (!f.is_open()) {
            cout << "Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\clearscript.puls" << endl;
            return 1;
        }
        string linef;
        while (getline(f, linef)) {
            cout << linef << endl;
        }
    }
    else {
        if (style == "normal") {
            system("cls");
            cout << CurrentPulsarInfo::title << endl;
        }
        else if (style == "big") {
            system("cls");
            cout << R"P(
  _____       _                
 |  __ \     | |               
 | |__) |   _| |___  __ _ _ __ 
 |  ___/ | | | / __|/ _` | '__|
 | |   | |_| | \__ \ (_| | |   
 |_|    \__,_|_|___/\__,_|_|
 
)P" << endl;
        }

        else if (style == "graffity") {
            system("cls");
            cout << R"P(
__________      .__                        
\______   \__ __|  |   ___________ _______ 
 |     ___/  |  \  |  /  ___/\__  \\_  __ \
 |    |   |  |  /  |__\___ \  / __ \|  | \/
 |____|   |____/|____/____  >(____  /__|   
                          \/      \/
)P" << endl;
        }
        else if (style == "epic") {
            system("cls");
            cout << R"P(

 _______           _        _______  _______  _______ 
(  ____ )|\     /|( \      (  ____ \(  ___  )(  ____ )
| (    )|| )   ( || (      | (    \/| (   ) || (    )|
| (____)|| |   | || |      | (_____ | (___) || (____)|
|  _____)| |   | || |      (_____  )|  ___  ||     __)
| (      | |   | || |            ) || (   ) || (\ (   
| )      | (___) || (____/\/\____) || )   ( || ) \ \__
|/       (_______)(_______/\_______)|/     \||/   \__/
                                                      
                  
)P" << endl;
        }
        else if (style == "speed") {
            system("cls");
            cout << R"P(           
    ____        __               
   / __ \__  __/ /________ ______
  / /_/ / / / / / ___/ __ `/ ___/
 / ____/ /_/ / (__  ) /_/ / /    
/_/    \__,_/_/____/\__,_/_/     
                                 
                                   
                  
)P" << endl;
        }
        else if (style == "3d") {
            system("cls");
            cout << R"P(           

      ___           ___           ___       ___           ___           ___     
     /\  \         /\__\         /\__\     /\  \         /\  \         /\  \    
    /::\  \       /:/  /        /:/  /    /::\  \       /::\  \       /::\  \   
   /:/\:\  \     /:/  /        /:/  /    /:/\ \  \     /:/\:\  \     /:/\:\  \  
  /::\~\:\  \   /:/  /  ___   /:/  /    _\:\~\ \  \   /::\~\:\  \   /::\~\:\  \ 
 /:/\:\ \:\__\ /:/__/  /\__\ /:/__/    /\ \:\ \ \__\ /:/\:\ \:\__\ /:/\:\ \:\__\
 \/__\:\/:/  / \:\  \ /:/  / \:\  \    \:\ \:\ \/__/ \/__\:\/:/  / \/_|::\/:/  /
      \::/  /   \:\  /:/  /   \:\  \    \:\ \:\__\        \::/  /     |:|::/  / 
       \/__/     \:\/:/  /     \:\  \    \:\/:/  /        /:/  /      |:|\/__/  
                  \::/  /       \:\__\    \::/  /        /:/  /       |:|  |    
                   \/__/         \/__/     \/__/         \/__/         \|__|    
                                  
                  
)P" << endl;
        }
        else {
            system("cls");
            cout << CurrentPulsarInfo::title << endl;
        }
    }
}

int neuro() {
    string pathToNeuro = "cd " + current_path + "\\SystemPuls\\systemmodules && pulsneuro.exe";
    system(pathToNeuro.c_str());
}

int AccountCommand(string line) {
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
        if (filesystem::exists(newDirecrotyAccount)) {
            cout << "Такой аккаунт уже существует" << endl;
            return 0;
        }
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
                configAnalyze(build_ID, name, password);
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
    else if (line.starts_with("change password")) {
        string check_true_password;
        cout << "Введите пароль от аккаунта: ";
        getline(cin, check_true_password);
        if (CurrentPulsarInfo::password == check_true_password) {
            cout << "Введите новый пароль от аккаунта: ";
            string newPassword;
            getline(cin, newPassword);
            ofstream f;
            f.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\password.ppas", ios::trunc);
            if (!f.is_open()) {
                cout << "Ошибка смены пароля" << endl;
            }
            else {
                f << newPassword;
                configAnalyze(build_ID, CurrentPulsarInfo::account, newPassword);
            }
            f.close();
        }
        else {
            cout << "Неверный пароль" << endl;
        }
    }



}

int configAnalyze(string bildn, string AccountName, string pas) {
    CurrentPulsarInfo::bildingid = bildn;
    CurrentPulsarInfo::account = AccountName;
    CurrentPulsarInfo::password = pas;
    string setConsoleTitlePuls = "title PulsarVenv " + AccountName;
    system(setConsoleTitlePuls.c_str());
    build_ID = bildn;
    filesystem::path tfp = filesystem::current_path();
    current_path = tfp.string();
    string color;
    fstream f;
    string pathToCfg = current_path + "\\accounts\\" + AccountName + "\\accountcfg\\style.pcfg";
    f.open(pathToCfg, fstream::in | fstream::out | fstream::app);
    if (!f.is_open()) {
        cout << "Ошибка при работе с файлом style.pcfg" << endl;
    }
    getline(f, color);
    getline(f, style);
    f.close();
    if (color != "normal") {
        string setColor = "color " + color;
        system(setColor.c_str());
    }
    return 0;
}

int comAnalyze(string line) {
    if (isLog) {
        writeInLog(line, false, false);
    }
    if (line.empty()) {
        return 0;
    }
    else if (line == "exit") {
        return 0101;
    }
    else if (line.starts_with("neuro")) {
        neuro();
    }
    else if (line.substr(0, 2) == "fs") {
        fileCommand(line, current_path, CurrentPulsarInfo::account);
    }
    else if (line.substr(0, 4) == "calc") {
        if (line.length() > 4) {
            puls_calc(line);
        }
        else {
            cout << "Ошибка: Введите выражение для вычисления (например: calc 2+2)" << endl;
        }
    }
    else if (line == "clear") {
        PulsarConsoleClear();
    }
    else if (line == "pinfo") {
        CurrentPulsarInfo::ShowInfo();
    }
    else if (line.substr(0, 9) == "sysconfig") {
        puls_sysconfig(line);
    }
    else if (line == "help" || line.substr(0, 4) == "help") {
        show_help();
    }
    else if (line.substr(0, 7) == "account") {
        AccountCommand(line);
    }
    else if (line.substr(0, 12) == "change style") {
        changeStyle();
    }
    else if (line == "chemdb") {
        string pathToChemBd = "cd " + current_path + "\\SystemPuls\\systemmodules && chembd.exe";
        system(pathToChemBd.c_str());
        PulsarConsoleClear();

    }
    else if (line.substr(0, 3) == "log") {
        line.replace(0, 3, "");
        line.erase(0, line.find_first_not_of(' '));
        line.erase(line.find_last_not_of(' ') + 1);
        if (line.starts_with("off")) {
            if (isLog == false) {
                cout << "Логирование команд уже выключено " << endl;
                return 0;
            }
            cout << "Логирование команд выключено " << endl;
            isLog = false;
        }
        else if (line.starts_with("on")) {
            if (isLog == true) {
                cout << "Логирование команд уже включено " << endl;
                return 0;
            }
            cout << "Логирование команд включено " << endl;
            isLog = true;
        }
        else if (line.starts_with("delete")) {
            string answer;
            while (true) {
                cout << "Очистка лога команд - опасна операция. \n Код опасности 1; \n Выполнить операцию? [Y/n] :";
                cin >> answer;
                if (answer == "Y") {
                    writeInLog("$pulsar log delete", true, false);
                    cout << "Лог команд очищен" << endl;
                    break;
                }
                else if (answer == "n") {
                    cout << "Запрет доступа к операции. Отказ пользователя" << endl;
                    break;
                }
                else {
                    cout << "Неизвестная команда. Введите Y - согласие. n - отказ" << endl;
                }
            }
        }
        else if (line.starts_with("write")) {
            line.erase(0, 5);
            line.erase(0, line.find_first_not_of(' '));

            if (line.empty() || line[0] != '"') {
                cout << RED << BOLD << "Ошибка: ожидается '\"' в начале строки" << CLEAR << endl;
                return 1;
            }

            string log_message;
            bool quote_closed = false;
            int i = 1;

            for (; i < line.length(); i++) {
                if (line[i] == '"') {
                    quote_closed = true;
                    i++;
                    break;
                }
                log_message += line[i];
            }

            if (!quote_closed) {
                cout << RED << BOLD << "Ошибка: незакрытый строковый литерал" << CLEAR << endl;
                return 1;
            }

            line.erase(0, i);
            line.erase(0, line.find_first_not_of(' '));

            bool add_time = false;
            if (line.starts_with("+time")) {
                add_time = true;
                line.erase(0, 5);
                line.erase(0, line.find_first_not_of(' '));
            }

            for (char c : line) {
                if (!isspace(c)) {
                    cout << RED << BOLD << "Ошибка: неожиданные символы после команды: '" << c << "'" << CLEAR << endl;
                    return 1;
                }
            }
            writeInLog(log_message, add_time, false);
        }

        else {
            cout << "Неопознанная log-команда" << endl;
        }

    }
    else if (line.starts_with("console.print")) {
        line.replace(0, 14, "");
        cout << line << endl;
    }
    else {
        string checkfile;
        if (line.find(".exe") != string::npos) {
            checkfile = current_path + "\\modules\\" + line;
        }
        else {
            checkfile = current_path + "\\modules\\" + line + ".exe";
        }

        if (filesystem::exists(checkfile)) {
            string cdmodule = "cd " + current_path + "\\modules && " + line;
            system(cdmodule.c_str());
        }
        else {
           cout << RED  << "Ошибка: Команда не распознана. Введите 'help' для справки." << CLEAR << endl;
        }
    }
}

int executePulsarScript(string line) {
    string comInFile;
    if (filesystem::exists(line)) {
        fstream scrFile;
        scrFile.open(line, fstream::in | fstream::out | ios::app);
        if (!scrFile.is_open()) {
            cout << "Ошибка работы с файлом - " << line << endl;
            return 1;
        }
        while (getline(scrFile, comInFile)) {
            comInFile.erase(0, comInFile.find_first_not_of(' '));
            comInFile.erase(comInFile.find_last_not_of(' ') + 1);
            int code = comAnalyze(comInFile);
            if (code == 0101) {
                scrFile.close();
                return 0101;
            }
        }
        scrFile.close();


    }

}

int pulsStarterScript() {
    string comInFile;
    if (filesystem::exists(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.txt")) {
        fstream scrsFile;
        scrsFile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.txt", fstream::in | fstream::out | ios::app);
        if (!scrsFile.is_open()) {
            cout << RED <<"Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.txt" << CLEAR << endl;
            return 1;
        }
        while (getline(scrsFile, comInFile)) {
            comInFile.erase(0, comInFile.find_first_not_of(' '));
            comInFile.erase(comInFile.find_last_not_of(' ') + 1);
            int code = comAnalyze(comInFile);
            if (code == 0101) {
                scrsFile.close();
                return 0101;
            }
        }
        scrsFile.close();
    }
    else if (filesystem::exists(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.puls")) {
        fstream scrsFile;
        scrsFile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.puls", fstream::in | fstream::out | ios::app);
        if (!scrsFile.is_open()) {
            cout << RED <<"Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.puls" << CLEAR << endl;
            return 1;
        }
        while (getline(scrsFile, comInFile)) {
            comInFile.erase(0, comInFile.find_first_not_of(' '));
            comInFile.erase(comInFile.find_last_not_of(' ') + 1);
            int code = comAnalyze(comInFile);
            if (code == 0101) {
                scrsFile.close();
                return 0101;
            }
        }
        scrsFile.close();
    }
}