#include <vector>
#include <string>
#include "split.h"

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

std::string remove_quotes(std::string str) {
    if (!str.empty()) {
        if (str.front() == '"' || str.front() == '\'') {
            str.erase(0, 1);
        }

        if (!str.empty() && (str.back() == '"' || str.back() == '\'')) {
            str.pop_back();
        }
    }
    return str;
}