// Библиотеки/модули необходимое для работы
#include "linuxPulsar.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

// Макросы цветов
#define CLEAR "\x1b[0m"
#define RED "\x1b[31m"	
#define GREEN "\x1b[32m"
#define BOLD "\x1b[1m"

// Пространство имен
using namespace std;

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


// Cкелеты/прототипы функций
int commandAnalyze(string line);

// Основная функция
int pulsarStart(){
    string com;
    while (true) {
        getline(cin, com);
        com.erase(0, com.find_first_not_of(' '));
        com.erase(com.find_last_not_of(' ') + 1);
        int code = commandAnalyze(com);
        if (code == 0101) { return 0101; }
    }
    return 0;
}

//Реализация функций
int commandAnalyze(string line) {
    if (line.empty()) {
        return 0;
    }
    else if (line.starts_with("//")) {
        return 0;
    }
    else if (line == "exit") {
        return 0101;
    }
    else {
        cout << RED <<"Ошибка: Команда не распознана. Введите 'help' для справки." << CLEAR<<endl;
    }
}