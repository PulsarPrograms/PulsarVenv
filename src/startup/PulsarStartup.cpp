
#include "PulsarStartup.h"

#include <clocale>
#include <iostream>
#include <string>
#include <filesystem>


using namespace std;

PulsarStartup::PulsarStartup() {
    PulsarStartup::current_path = filesystem::current_path().string();
    PulsarStartup::version = "1.0.0";
}


std::string PulsarStartup::get_version() {
    return  PulsarStartup::version;
}

bool PulsarStartup::check_have_directories() {
    bool is_have_directories = true;
    string directories[]{PulsarStartup::current_path + "\\system", PulsarStartup::current_path + "\\system\\profiles"};
    for (auto directory : directories) {
        if (!(filesystem::exists(directory))) {
            cout << "PulsarStartup error:  Отстутвует системная директория -- " << directory << endl;
            is_have_directories = false;
        }
    }
    return is_have_directories;
}



