#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "ru");
	if (argc < 2) {
		cout << "���������� ��������� ���������. help - ��� ������� ������" << endl;
	}
	if (argv[1] == "help") {
		cout << "add - �������� �������" << endl;
		/*cout << endl;
		cout << endl;
		cout << endl;
		cout << endl;*/

	} else if (argv[1] == "add") {
		if (argc < 4) {
			cout << "���������� ��������� ���������.  ������ - ������� - ��� �������� - ������" << endl;
			cout << "���� ������ ������� ������� ��� ������ ������� \"none\"" << endl;
		}
		/*string newDirecrotyAccount = curpath + "\\accounts\\" + argv[2];
		filesystem::create_directory(newDirecrotyAccount);
		filesystem::create_directory(newDirecrotyAccount + "\\accountcfg");
		filesystem::create_directory(newDirecrotyAccount + "\\userfiles");
		ofstream f;
		f.open(newDirecrotyAccount + "\\accountcfg\\password.ppas");
		f << argv[3];
		f.close();
		cout << "������� ������ �������\n��� - " << name << "     ������ - " << password << endl;
		this_thread::sleep_for(std::chrono::milliseconds(1100));
		system("cls");
	} else if (argv[1] )*/
	}
	system("pause>nul");
	return 0;
}