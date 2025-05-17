#include "clear_screen.h"

#include <cstdlib>

void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear")
    #endif
}
