#include <string>
#include <vector>
#include <chrono>
#include "utils.h"

#include <iostream>

#include "PulsarCore.h"

using namespace std;

void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
    #endif
}


string get_current_date_time() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;
#ifdef _WIN32
    localtime_s(&local_time, &now_time);
#else
    localtime_r(&now_time, &local_time);
#endif

    stringstream datetime_stream;
    datetime_stream << put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return datetime_stream.str();
}

vector<string> split(const string& str) {
    vector<string> tokens;
    string current;
    bool in_quotes = false;
    char quote_char = '\0';

    for (char c : str) {
        if ((c == '"' || c == '\'') && !in_quotes) {
            in_quotes = true;
            quote_char = c;
            continue;
        } else if (in_quotes && c == quote_char) {
            in_quotes = false;
            continue;
        }

        if (isspace(c) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

string remove_quotes(string str) {
    if (str.length() >= 2) {
        const char first = str.front();
        const char last = str.back();

        if ((first == '"' && last == '"') || (first == '\'' && last == '\'')) {
            str.erase(0, 1);
            str.pop_back();
        }
    }
    return str;
}

string strip(const std::string &str) {
    return std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
}

#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GREY    "\033[90m"
#define PURPLE  "\033[35m"
#define PINK "\033[95m"
#define NEON_CYAN "\033[96m"
#endif

void set_color(int color=7) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
#else
    switch (color) {
        case 12: std::cout << RED; break;
        case 10: std::cout << GREEN; break;
        case 14: std::cout << YELLOW; break;
        case 9:  std::cout << BLUE; break;
        case 13: std::cout << MAGENTA; break;
        case 11: std::cout << CYAN; break;
        case 15: std::cout << WHITE; break;
        case 7:  std::cout << RESET; break;
        case 8:  std::cout << GREY; break;
        case 5:  std::cout << PURPLE; break;
        default: std::cout << RESET; break;
    }
#endif
}

void cout_err(const string &msg) {
    set_color(12);
    cerr << msg << endl;
    set_color(7);
}

void cout_good(const string &msg){
    set_color(10);
    cerr << msg << endl;
    set_color(7);
}

