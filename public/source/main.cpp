#include <iostream>
#include <string>
#include <dos.h>
#include "wp.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <Windows.h>;

using namespace std;



int main(){
    
    fstream curfile;
    int code = -1;
    setlocale(LC_ALL, "Ru");
    cout << "______                                ________     _________      ______ " << endl;
    cout << "|     |    |      /|     |           |            |        |      |     |" << endl;
    cout << "|_____|    |    /  |     |           |_________   |________|      |_____|      " << endl;
    cout << "|          |  /    |     |                    |   |        |      | \\" << endl;
    cout << "|          |/      |     |________    ________|   |        |      |    \\" << endl;
    cout << endl;
    cout << endl;
    cout << "PulsarVenv инициализирован\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    cout << "Загрузка...\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    curfile.open("maincfg.pcfg", fstream::in | fstream::out | fstream::app);
    string buildid;
    if (!curfile.is_open()) { 
        cout << "Ошибка при работе с файлом maincfg.pcfg" << endl; 
    }
    else
    {
        string line;
        string build_number;
        if (getline(curfile, line)) {
            size_t pos = line.find("build id: ");
            if (pos != string::npos) {
                // Извлекаем только номер
                build_number = line.substr(pos + 9);
                // Удаляем возможные пробелы в начале и конце
                build_number.erase(0, build_number.find_first_not_of(" \t"));
                build_number.erase(build_number.find_last_not_of(" \t") + 1);
            }
        }
            #ifdef _WIN32
                    cout << "Windows система определена\n";
                    this_thread::sleep_for(std::chrono::milliseconds(300));
                    code = pulsarstart(build_number);
            #elif __linux__
        cout << "Linux система определена\n" << endl;
            cout << "Необнаружено версий PulsarVenv для Linux" << endl;
        cin.get();
            #endif
    }
    return 0;

}


