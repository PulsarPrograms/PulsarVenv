#include <iostream>
#include <string>
#include <dos.h>
#include "wp.h"
#include <chrono>
#include <thread>

using namespace std;

int main()
{
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
    #ifdef _WIN32
    cout << "Windows система определена\n";
    this_thread::sleep_for(std::chrono::milliseconds(300));
    code = pulsarstart();
    #elif __linux__
    cout << "Linux система определена\n"
        cout << "Необнаружено версий PulsarVenv для Linux"
#endif
    return 0;

}


