/* PulsarPrograms Company (Github: https://github.com/PulsarPrograms)
 * Product - PulsarVenv (simple VirtualEnv) (Github:https://github.com/PulsarPrograms/PulsarVenv)
 * Author - zeroqxq (Github:  https://github.com/zeroqxq)
 */


#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "startup/PulsarStartup.h"
#include "core/PulsarCore.h"
#include "profile/PulsarProfileManager.h"
#include "../commandHandler/CommandHandler.h"



using namespace std;
/* Чтобы не раскидывать мини функции по аддонам добавил их в main. */

void normalize_locale() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(65001);
#endif
    setlocale(LC_ALL, ".UTF8");
}

bool is_running_under_wine() {
    const char* wineLoader = std::getenv("WINELOADER");
    const char* winePrefix = std::getenv("WINEPREFIX");
    return (wineLoader != nullptr || winePrefix != nullptr);
}


string get_current_os() {
    if (is_running_under_wine()) {
        return "Wine";
    }
    else  {
#ifdef _WIN32
        return "Windows";
#elif __linux__
        return "Linux";
#elif __APPLE__
        return "macOS";
#else
        return "Unix-подобная OS";
#endif
    }
}



/* Функция main тоже относится к PulsarStartup, выполняет связь всех компонентов. Второе ядро, связующее все компоненты*/
int main(int argc, char* argv[]) {
        // Устанавливаем локаль
        normalize_locale();
        // Инициализация всех нужных компонентов
        PulsarStartup pulsar;
        if (!pulsar.check_have_directories()){ cin.get(); return 1; }

        // Самый главный компонент - ядро. Другие компоненты будут использовать функционал ядра
        PulsarCore core;
        PulsarCore::launch_time = pulsar.launch_time;
        PulsarCore::start_time = pulsar.start_time;
        PulsarCore::current_path = pulsar.current_path; // Путь до папки с пульсаром
        PulsarCore::pulsar_locale = pulsar.pulsar_locale; // Сам томл файл локализации
        PulsarCore::platform = get_current_os(); // Платформа
        PulsarCore::version = pulsar.get_version(); // Версия


        PulsarProfileManager manager; // Менеджер профилей
        manager.setup_accounts(); // Устанавливаем имена аккаунтов
        // Аргументы
        if (argc > 2) {
            bool register_or_login = false;
            for (int i = 1; i < argc; ++i) {
                string arg = argv[i];
                if (arg == "-l" && i + 1 < argc) {
                    if (manager.login_profile(argv[i + 1]) != 0) {
                        cerr << "Нажмите Enter для выхода...";
                        cin.get();
                        return 1;
                    }
                    i++;
                    register_or_login = true;
                }
                else if (arg == "-r" && i + 1 < argc) {
                    manager.register_profile(argv[i + 1]);
                    i++;
                    register_or_login = true;
                }
                else if (arg == "-c" && i + 1 < argc) {
                    if (!(register_or_login)) {
                        cerr << "Register or login in to your profile before using" << endl;
                    }
                    for (int j = i + 1; j < argc; ++j) {
                        CommandHandler::execute(argv[j]);
                    }
                    break;
                }
            }

        }
        else {

            if (manager.account_names.size() == 0) {
                string name;
                cout << "Creating a profile operation" << endl;
                cout << PulsarCore::pulsar_locale["enter_name_profile"].value_or("Enter the profile name: ")<< ": "; getline(cin, name);
                manager.register_profile(name);
            }
            else {
                string name;
                cout << PulsarCore::pulsar_locale["enter_name_profile"].value_or("Enter the profile name: ") << ": "; getline(cin, name);
                if (manager.login_profile(name) != 0) {
                    cerr << "Нажмите Enter для выхода...";
                    cin.get();
                    return 1;
                }
            }
            if (core.start() != 0) {
                cerr << "Нажмите Enter для выхода...";
                cin.get();
                return 1;
            }

        }

    return 0;

}

