#include "linuxPulsar.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;


int commandAnalyze(string line){
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
         cout << "Ошибка: Команда не распознана. Введите 'help' для справки." << endl;
    }
}

int pulsarStart(){
    string com;
    while (true) {
        getline(cin, com);
        com.erase(0, com.find_first_not_of(' '));
        com.erase(com.find_last_not_of(' ') + 1);
        int code = commandAnalyze(com);
        if(code==0101){}
    }
    return 0;
}
