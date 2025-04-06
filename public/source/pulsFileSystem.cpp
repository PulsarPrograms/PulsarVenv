#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;

int fileCommand(std::string line, std::string curpath, std::string acc_name) {
	line.replace(0, 2, "");
	line.erase(0, line.find_first_not_of(' '));
	line.erase(line.find_last_not_of(' ') + 1);
	if (line.substr(0, 4) == "help") {
		cout << "md - создать дирректорию" << endl;
		cout << "ld - показать все дирректории и файлы в них" << endl;
	}
	if (line.substr(0, 2) == "md") {
		line.replace(0, 2, "");
		line.erase(0, line.find_first_not_of(' '));
		line.erase(line.find_last_not_of(' ') + 1);
		string kav = "\"";
		string whereCreateDir = "md " + kav + curpath + "\\accounts\\" + acc_name + "\\userfiles\\" + line + "\"";
		system(whereCreateDir.c_str());


	}
	else if (line.substr(0, 2) == "ld") {
		for (const auto& entry : filesystem::recursive_directory_iterator(curpath + "\\accounts\\" + acc_name + "\\userfiles")) {
			string ns = entry.path().string();
			string repl = curpath + "\\accounts\\" + acc_name + "\\userfiles";
			ns.replace(0, repl.length(), acc_name + "\\");
			cout << ns << endl;
		}

	}
	return 0;
}
