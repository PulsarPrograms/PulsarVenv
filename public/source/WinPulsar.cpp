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
#include <cctype>
#include "C:\Users\user\source\repos\PulsarVenv\PulsarVenv\pulsFileSystem.h"
using namespace std;
string current_path;
string build_ID;
bool isStandartStyle = true;
//TODO  смена имени аккаугта \ создание файлов + работа с ними
/*Коды возвращаемых ошибок
0001 - код выхода из системы*/

atomic<bool> timeThreadRunning(true);
mutex consoleMutex;

void PulsarConsoleClear();
int configAnalyze(string bildn, string AccountName, string pas);
bool isValidHexColor(string s);

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
string CurrentPulsarInfo::title = "PulsarVenv 0.0.1-alpha";
string CurrentPulsarInfo::platform_version = "Windows";
string CurrentPulsarInfo::account = "";
int CurrentPulsarInfo::start_time = 0;
string CurrentPulsarInfo::bildingid = "";
string CurrentPulsarInfo::password = "";

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
        cout << "Введите стиль интерфейса (big/normal): ";
        getline(cin, newStyle);
        if (newStyle != "big" && newStyle != "normal") {
            cout << "Неправильный ввод" << endl;
            continue;
        }
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
    string comforsys = "cd " + current_path + "\\\\SytemPuls\\systemmodules && pulsarcalc.exe " + line;
    system(comforsys.c_str());
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
    cout << "PulsarVenv 0.0.1-alpha - Справочник по командам" << endl << endl;

    cout << "Основные команды:" << endl;
    cout << "=================" << endl;
    cout << "help               - Показать эту справку" << endl;
    cout << "exit               - Выйти из системы" << endl;
    cout << "clear              - Очистить экран консоли" << endl;
    cout << "pinfo              - Показать информацию о системе (версия, время работы и т.д.)" << endl << endl;

    cout << "Команды работы с системой:" << endl;
    cout << "=========================" << endl;
    cout << "sysconfig          - Показать конфигурацию системы" << endl;
    cout << "sumulator_pulsar   - Запустить эмуляцию пульсара (требует подтверждения)" << endl << endl;

    cout << "Команды для вычислений:" << endl;
    cout << "=======================" << endl;
    cout << "calc <выражение>   - Выполнить математическое вычисление (например: calc 2+2*3)" << endl << endl;

    cout << "Работа с Python:" << endl;
    cout << "===============" << endl;
    cout << "python <скрипт>    - Запустить Python скрипт (доступно только в сборке 0000)" << endl << endl;

    cout << "Управление аккаунтами:" << endl;
    cout << "=====================" << endl;
    cout << "account add        - Создать новый аккаунт" << endl;
    cout << "account remove     - Удалить существующий аккаунт" << endl;
    cout << "account swap       - Сменить текущий аккаунт" << endl << endl;

    cout << "Запуск программ:" << endl;
    cout << "===============" << endl;
    cout << "<имя_программы>    - Запустить программу из папки modules" << endl;
    cout << "                    (можно указывать с расширением .exe или без)" << endl;
}

void PulsarConsoleClear() {
    if (isStandartStyle == true) {
        system("cls");
        cout << CurrentPulsarInfo::title << endl;
    }
    else {
        system("cls");
        cout << "______                                ________     _________      ______ " << endl;
        cout << "|     |    |      /|     |           |            |        |      |     |" << endl;
        cout << "|_____|    |    /  |     |           |_________   |________|      |_____|      " << endl;
        cout << "|          |  /    |     |                    |   |        |      | \\" << endl;
        cout << "|          |/      |     |________    ________|   |        |      |    \\" << endl;
        cout << endl;
        cout << endl;
    }
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
    else if(line.starts_with("change password")){
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
            } else {
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
    string color, style;
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
    } if (style == "big") {
        isStandartStyle = false;
    }
    return 0;
}
    
int pulsarstart(string bildn, string AccountName, string pas) {
    setlocale(LC_ALL, "Ru");
    CurrentPulsarInfo::start_time = clock();
    configAnalyze(bildn, AccountName, pas);
    string com;
    system("cls");
    PulsarConsoleClear();
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
        else if (com.substr(0, 2) == "fs") {
            fileCommand(com, current_path, CurrentPulsarInfo::account);
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
        else if (com.substr(0, 12) == "change style") {
            changeStyle();
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