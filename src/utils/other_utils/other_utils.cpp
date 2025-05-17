#include "other_utils.h"
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

string getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_time);

    stringstream datetime_stream;
    datetime_stream << put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return datetime_stream.str();
}