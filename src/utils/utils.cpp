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
    std::system("clear")
    #endif
}


string get_current_date_time() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_time);

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