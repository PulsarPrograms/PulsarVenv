#include "PulsarStartup.h"

#include <clocale>
#include <iostream>
#include <string>
#include <filesystem>
#include <../utils/utils.h>


using namespace std;

PulsarStartup::PulsarStartup() {
    PulsarStartup::start_time = clock();
    PulsarStartup::current_path = filesystem::current_path().string();
    PulsarStartup::version = "v1.0.0 Private BETA";
    PulsarStartup::launch_time = get_current_date_time();
}


std::string PulsarStartup::get_version() {
    return  PulsarStartup::version;
}
//.
bool PulsarStartup::check_have_directories() {
    bool is_have_directories = true;
    string directories[]{PulsarStartup::current_path + "/system", PulsarStartup::current_path + "/system/profiles" , PulsarStartup::current_path + "/system/themes", PulsarStartup::current_path + "/system/systemmodules", PulsarStartup::current_path + "/system/locale", PulsarStartup::current_path + "/system/pulsfs",PulsarStartup::current_path + "/system/pulsfs/pulsarvenv/home",
    PulsarStartup::current_path + "/system/maincfg.toml",};
    for (auto directory : directories) {
        if (!(filesystem::exists(directory))) {
            cout_err(pulsar_locale["not_found_directory"].value_or("PulsarStartup error: The system directory is missing -- ")  + directory);
            is_have_directories = false;
        }
    }
    if (is_have_directories) PulsarStartup::pulsar_locale = toml::parse_file(PulsarStartup::current_path + "/system/locale/standard_locale.toml");
    return is_have_directories;
}



