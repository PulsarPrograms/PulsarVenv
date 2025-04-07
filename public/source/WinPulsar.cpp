#include <iostream>
#include <string>
#include "wp.h"
#include <cstdlib>
#include <filesystem>

using namespace std;

/*Коды возвращяемых ошибок
0001 - код выхода из системы*/

int pulsarstart() {
	setlocale(LC_ALL, "Ru");
	string title = "PulsarVenv 0.0.1-alpha";
	string com;
	system("cls");
	cout << title << endl;
	while (true) {
		cout << "$> ";
		getline(cin, com);
		auto new_end = remove(com.begin(), com.end(), ' ');
		com.erase(new_end, com.end());
		if (com == "") {
			continue;
		}
		else if (com == "exit") {
			return 0101;
		}
		else if (com.substr(0, 4) == "calc") {
			com.replace(0, 4, " ");
			auto new_end = remove(com.begin(), com.end(), ' ');
			com.erase(new_end, com.end());
			filesystem::path tfp = filesystem::current_path();
			string tfp1 = tfp.string();
			string comforsys = "cd";
			comforsys = comforsys + " " + tfp1;
			system(comforsys.c_str());
			string comforcalc = "pulsarcalc.exe";
			comforcalc = comforcalc + " " + com;
			system(comforcalc.c_str());
		}
		else if (com == "clear") {
			system("cls");
			cout << title << endl;
		}
		else {
			cout << "Команда нераспознана. Проверьте правильность написания" << endl;
		}
			
		
	}
}