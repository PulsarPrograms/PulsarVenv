#include <iostream>
#include <string>
#include "wp.h"
#include <cstdlib>
#include <filesystem>
#include <ctime> 

using namespace std;

/*Коды возвращяемых ошибок
0001 - код выхода из системы*/

class CurrentPulsarInfo {
public:
	string title = "PulsarVenv 0.0.1-alpha";
	string platform_version = "Windows";
	string account = "Guest";
	int start_time = 0;
	string bildingid;

	void ShowInfo() {
		unsigned int point_time = clock();
		cout << "-----------------------------------------" << endl;
		cout << "Version:     " << title << endl;
		cout << "Platform:    " << platform_version << endl;
		cout << "Working time " << (point_time - start_time)/1000 << " sec" << endl;
		cout << "Account:     " << account << endl;
		cout << "Build id:    " << bildingid << endl;
		cout << "-----------------------------------------" << endl;
	}
};

int pulsarstart(string bildn) {
	setlocale(LC_ALL, "Ru");
	CurrentPulsarInfo session;
	session.start_time = clock();
	session.bildingid = bildn;
	string com;
	filesystem::path tfp = filesystem::current_path();
	string curpath = tfp.string();
	system("cls");
	cout << session.title << endl;
	while (true) {
		cout << "$> ";
		getline(cin, com);
		size_t first_non_space = com.find_first_not_of(' ');
		if (first_non_space != std::string::npos) {
			com.erase(0, first_non_space);
		}
		else {
			com.clear();
		}
		if (com == "") {
			continue;
		}
		else if (com == "exit") {
			return 0101;
		}
		else if (com.substr(0, 4) == "calc") {
			com.replace(0, 4, "");
			size_t first_non_space = com.find_first_not_of(' ');
			if (first_non_space != string::npos) {
				com.erase(0, first_non_space);
			}
			string comforsys = "cd ";
			comforsys = comforsys + " " + curpath + "\\modules " + "&&" + "pulsarcalc.exe " + com;
			system(comforsys.c_str());
		}
		else if (com == "clear") {
			system("cls");
			cout << session.title << endl;
		}
		else if (com == "pinfo") {
			session.ShowInfo();
		}
		else if (com.substr(0, 7) == "python") {
			if (bildn != "0000") { cout << "В данной сборке отсутсвует эта команда..." << endl; }
			else {
				com.replace(0, 7, "");
				size_t first_non_space = com.find_first_not_of(' ');
				if (first_non_space != string::npos) {
					com.erase(0, first_non_space);
				}
				string pfile = "cd " + curpath + "\\python313puls && ppython.exe " + com;
				system(pfile.c_str());
			}
		}
		else if (com.substr(0, 9) == "sysconfig") {
			com.replace(0, 9, "");
			size_t first_non_space = com.find_first_not_of(' ');
			if (first_non_space != string::npos) {
				com.erase(0, first_non_space);
			}
			string scfile = "cd " + curpath + "\\modules && sysconfig.exe";
			cout << "Определение параметров системы... Пожалуйста подождите...\n"<< endl;
			system(scfile.c_str());
		}
		else if (com.substr(0, 4) == "help") {
			cout << "Тут будет ссылка...." << endl;
		}
		else {
			string checkfile;
			if (com.contains(".exe")) {
				checkfile = curpath + "\\modules\\" + com;
			}
			else {
				checkfile = curpath + "\\modules\\" + com + ".exe";
			}
			if (filesystem::exists(checkfile) == true) {
				string cdmodule = "cd " + curpath + "\\modules " + "&& " + com;
				system(cdmodule.c_str());
			}
			else {
				cout << "Команда нераспознана. Проверьте правильность написания" << endl;
			}
		}
			
		
	}
}