#include "winPulsar.h"
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
string style = "";
bool isLog = true;
bool isError = true;
//TODO  смена имени аккаугта \ создание файлов + работа с ними
/*Коды возвращаемых ошибок
0001 - код выхода из системы*/

atomic<bool> timeThreadRunning(true);
mutex consoleMutex;

int PulsarConsoleClear();
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
string CurrentPulsarInfo::title = "PulsarVenv 0.2.5";
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

void sumulator_pulsar() {
    string pathToSumPuls = "cd " + current_path + "\\SystemPuls\\systemmodules && pulsarem.exe";
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
    cout << "pinfo              - Показать информацию о Pulsar (версия, время работы и т.д.)" << endl << endl;
    cout << "сруьви             - Химическая база данных" << endl << endl;

    cout << "Команды работы с системой:" << endl;
    cout << "=========================" << endl;
    cout << "sysconfig          - Показать конфигурацию системы" << endl;
    cout << "sumulator_pulsar   - Запустить эмуляцию пульсара (требует подтверждения)" << endl << endl;

    cout << "Команды для вычислений:" << endl;
    cout << "=======================" << endl;
    cout << "calc <выражение>   - Выполнить математическое вычисление (например: calc 2+2*3)" << endl << endl;

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
    ofstream logfile;
    if (isLog) {
        logfile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\log.plog", fstream::in | fstream::out | ios::app);
        logfile << line;
        logfile << "\n";
        logfile.close();
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
    else if (line == "sumulator_pulsar") {
        sumulator_pulsar();
    }
    else if (line.substr(0, 7) == "account") {
        AccountCommand(line);
    }
    else if (line.substr(0, 12) == "change style") {
        changeStyle();
    }
    else if (line.starts_with("-error")) {
        cout << "Сообщения об ошибках выключены" << endl;
        isError = false;
    }
    else if (line.starts_with("+error")) {
        cout << "Сообщения об ошибках включены" << endl;
        isError = true;
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
                return 0; }
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
                    logfile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\log.plog", fstream::in | fstream::out | ios::trunc);
                    logfile << "#pulsar log delete " + CurrentPulsarInfo::getCurrentDateTime();
                    logfile << "\n";
                    logfile.close();
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
            if(isError) cout << "Ошибка: Команда не распознана. Введите 'help' для справки." << endl;
        }
    }
}

int startPulsScript(string line) {
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
            cout << "Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.txt" << endl;
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
            cout << "Ошибка работы с файлом - " << current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\startscript.puls" << endl;
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

int pulsarStart(string bildn, string AccountName, string pas) {
    setlocale(LC_ALL, "Ru");
    ofstream logfile;
    CurrentPulsarInfo::start_time = clock();
    configAnalyze(bildn, AccountName, pas);
    logfile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\log.plog", fstream::in | fstream::out | ios::app);
    logfile << "$startpulsar " + CurrentPulsarInfo::getCurrentDateTime();
    logfile << "\n";
    logfile.close();
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
            code = startPulsScript(com);
        }
        else {
            code = comAnalyze(com);
        }
        if (code == 0101) {
            logfile.open(current_path + "\\accounts\\" + CurrentPulsarInfo::account + "\\accountcfg\\log.plog", fstream::in | fstream::out | ios::app);
            logfile << "$exitpulsar " + CurrentPulsarInfo::getCurrentDateTime();
            logfile << "\n";
            logfile.close();
            return 0101; 
        }
       
    }
}