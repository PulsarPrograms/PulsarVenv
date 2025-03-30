#include <iostream>
#include <string>
#include "wp.h"
#include <cstdlib>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
using namespace std;
string curpath;
string build_ID;

/*���� ������������ ������
0001 - ��� ������ �� �������*/

atomic<bool> timeThreadRunning(true);
mutex consoleMutex;

class CurrentPulsarInfo {
public:
    string title = "PulsarVenv 0.0.1-alpha";
    string platform_version = "Windows";
    string account = "Guest";
    int start_time = 0;
    string bildingid;

    string getCurrentDateTime() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm local_time;
        localtime_s(&local_time, &now_time);

        stringstream datetime_stream;
        datetime_stream << put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        return datetime_stream.str();
    }

    void ShowInfo() {
        unsigned int point_time = clock();
        cout << "----------------------------------------------" << endl;
        cout << "������:         " << title << endl;
        cout << "���������:      " << platform_version << endl;
        cout << "����� ������:   " << (point_time - start_time) / 1000 << " ���" << endl;
        cout << "������� �����:  " << getCurrentDateTime() << endl;
        cout << "�������:        " << account << endl;
        cout << "ID ������:      " << bildingid << endl;
        cout << "----------------------------------------------" << endl;
    }
};

void puls_calc(string line) {
    line.replace(0, 4, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    string comforsys = "cd " + curpath + "\\modules && pulsarcalc.exe " + line;
    system(comforsys.c_str());
}

void puls_python(string line) {
    if (build_ID != "0000") {
        cout << "� ������ ������ ����������� ��� �������..." << endl;
    }
    else {
        line.replace(0, 7, "");
        size_t first_non_space = line.find_first_not_of(' ');
        if (first_non_space != string::npos) {
            line.erase(0, first_non_space);
        }
        string pfile = "cd " + curpath + "\\python313puls && ppython.exe " + line;
        system(pfile.c_str());
    }
}

void puls_sysconfig(string line) {
    line.replace(0, 9, "");
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != string::npos) {
        line.erase(0, first_non_space);
    }
    string scfile = "cd " + curpath + "\\modules && sysconfig.exe";
    cout << "����������� ���������� �������... ���������� ���������...\n" << endl;
    system(scfile.c_str());
}

void show_help() {
    cout << "\n��������� �������:" << endl;
    cout << "  calc <���������>  - ����������� (��������: calc 2+2)" << endl;
    cout << "  python <������>   - ������ Python ������� (������ ��� ������ 0000)" << endl;
    cout << "  sysconfig         - �������� ������������ �������" << endl;
    cout << "  pinfo             - �������� ���������� � �������" << endl;
    cout << "  clear             - �������� �����" << endl;
    cout << "  help              - �������� ��� �������" << endl;
    cout << "  exit              - ����� �� PulsarVenv" << endl;
    cout << "  <���_���������>   - ������ ��������� �� ����� modules\n" << endl;
}

int pulsarstart(string bildn) {
    setlocale(LC_ALL, "Ru");
    CurrentPulsarInfo session;
    session.start_time = clock();
    session.bildingid = bildn;
    build_ID = bildn;
    string com;
    filesystem::path tfp = filesystem::current_path();
    curpath = tfp.string();
    system("cls");
    cout << session.title << endl;

    while (true) {
        cout << "$> ";
        getline(cin, com);

        // �������� ������ ��������
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
                cout << "������: ������� ��������� ��� ���������� (��������: calc 2+2)" << endl;
            }
        }
        else if (com == "clear") {
            system("cls");
            cout << session.title << endl;
        }
        else if (com == "pinfo") {
            session.ShowInfo();
        }
        else if (com.substr(0, 7) == "python") {
            if (com.length() > 7) {
                puls_python(com);
            }
            else {
                cout << "������: ������� ������ ��� ������� (��������: python script.py)" << endl;
            }
        }
        else if (com.substr(0, 9) == "sysconfig") {
            puls_sysconfig(com);
        }
        else if (com == "help" || com.substr(0, 4) == "help") {
            show_help();
        }
        else {
            string checkfile;
            if (com.find(".exe") != string::npos) {
                checkfile = curpath + "\\modules\\" + com;
            }
            else {
                checkfile = curpath + "\\modules\\" + com + ".exe";
            }

            if (filesystem::exists(checkfile)) {
                string cdmodule = "cd " + curpath + "\\modules && " + com;
                system(cdmodule.c_str());
            }
            else {
                cout << "������: ������� �� ����������. ������� 'help' ��� �������." << endl;
            }
        }
    }
}