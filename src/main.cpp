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

bool is_running_on_linux() {
#ifdef __linux__
    return true;
#else
    return false;
#endif
}

bool is_running_on_windows() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

bool is_running_on_mac_os() {
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
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

void clear_screen() {
    (is_running_on_windows()) ? system("cls") : system("clear");
}

/* Функция main тоже относится к PulsarStartup, выполняет свызь всех компонентов*/
int main() {
    try {
        // Устанавливаем локаль
        normalize_locale();
        // Инициализация всех нужных компонентов
        PulsarStartup pulsar;
        if (!pulsar.check_have_directories()){ cin.get(); return 1; }

        // Самый главный компонент - ядро. Другие компоненты будут использовать функционал ядра
        PulsarCore core;
        PulsarCore::current_path = pulsar.current_path; // Путь до папки с пульсаром
        PulsarCore::pulsar_locale = pulsar.pulsar_locale; // Сам томл файл локализации
        core.set_platform(get_current_os()); // Платформа
        core.set_version(pulsar.get_version()); // Версия


        PulsarProfileManager manager; // Менеджер профилей
        manager.setup_accounts(); // Устанавливаем имена аккаунтов
        (manager.account_names.size() == 0) ? manager.register_profile() : manager.login_profile();
        core.start();
    } catch (string error) {
        cout << "PulsarError: " <<  error << endl;
    }
    return 0;

}