#include <iostream>
#include <string>

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
		return 1;
	}
	system("pause>nul");
	return 0;
}