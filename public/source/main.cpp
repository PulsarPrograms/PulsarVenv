#include <iostream>
#include <string>
#include <dos.h>
#include "wp.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <Windows.h>;
#include <filesystem>
#include <cstdlib>

using namespace std;
string name;
string password;
string curpath;

void PulsarLogo() {
    cout << "______                                ________     _________      ______ " << endl;
    cout << "|     |    |      /|     |           |            |        |      |     |" << endl;
    cout << "|_____|    |    /  |     |           |_________   |________|      |_____|      " << endl;
    cout << "|          |  /    |     |                    |   |        |      | \\" << endl;
    cout << "|          |/      |     |________    ________|   |        |      |    \\" << endl;
    cout << endl;
    cout << endl;
}

void CreateAccount() {
        cout << "Введите имя аккаунта: ";
        getline(cin, name);
        cout << "СПРАВКА - если хотите создать аккаунт без пароля укажите \"none\"" << endl;
        while (true){
            cout << "Введите пароль аккаунта: ";
            getline(cin, password);
            if (password.length() < 4) {
                cout << "\nПароль должен содержать минимум 4 символа";
                continue;
            }
            break;
        }
        string newDirecrotyAccount = curpath + "\\accounts\\" + name;
        filesystem::create_directory(newDirecrotyAccount);
        filesystem::create_directory(newDirecrotyAccount + "\\accountcfg");
        filesystem::create_directory(newDirecrotyAccount + "\\userfiles");
        ofstream f;
        f.open(newDirecrotyAccount + "\\accountcfg\\password.ppas");
        f << password;
        f.close();
        cout << "Успешно создан аккаунт\nИмя - " << name << "     Пароль - " << password << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1100));
        system("cls");
}

int LoginInAccount() {
    while (true) {
        cout << "Введите имя аккаунта: ";
        getline(cin, name);
        fstream f;
        f.open(curpath + "\\accounts\\" + name + "\\accountcfg\\password.ppas", fstream::in | fstream::out | fstream::app);
        string correctPassword;
        getline(f, correctPassword);
        if (!f.is_open()) {
            cout << "Такого аккаунта не существует или произшла ошибка" << endl;
            return 1;
        }
        f.close();
        if (correctPassword == "none") {
            cout << "Успешная авторизация" << endl;
            break;
        }
        cout << "Введите пароль аккаунта: ";
        getline(cin, password);
        if (correctPassword == password) {
            cout << "Успешная авторизация" << endl;
            break;
        }
        else {
            cout << "Неверный пароль. Попробуйте еще раз!\n";
            continue;
        }

    }
    return 0;
}



int main(){
    fstream curfile;
    filesystem::path currentPathtoFile = filesystem::current_path();
    curpath = currentPathtoFile.string();
    int code = -1;
    setlocale(LC_ALL, "Ru");
    PulsarLogo();
    cout << "PulsarVenv инициализирован\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    cout << "Загрузка...\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    string pathToMainCfg = curpath + "\\SystemPuls\\systemcfg\\maincfg.pcfg";
    curfile.open(pathToMainCfg, fstream::in | fstream::out | fstream::app);
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
                build_number = line.substr(pos + 9);
                build_number.erase(0, build_number.find_first_not_of(" \t"));
                build_number.erase(build_number.find_last_not_of(" \t") + 1);
            }
        }
            #ifdef _WIN32
                    cout << "Windows система определена\n";
                    this_thread::sleep_for(std::chrono::milliseconds(300));
                    system("cls");
                    PulsarLogo();
                    int countAccount = 0;
                    string accountDirectoryPath = curpath + "\\accounts";
                    for (const auto& entry : filesystem::directory_iterator(accountDirectoryPath)) {
                        if (entry.is_directory()) {
                            string currentAccount = entry.path().filename().string();
                            countAccount++;
                        }
                    }
                    if (countAccount == 0) {
                        cout << "У вас нет аккаунтов. Для продолжения необходимо создать аккаунт" << endl;
                        CreateAccount();
                    }
                    else {
                       int check = LoginInAccount();
                       if (check == 1) {
                           return 1;
                       }
                        
                    }
                    code = pulsarstart(build_number, name);
                    
            #elif __linux__
        cout << "Linux система определена\n" << endl;
            cout << "Необнаружено версий PulsarVenv для Linux" << endl;
        cin.get();
            #endif
    }
    return 0;

}


