#include <string>
#include <vector>
#include <chrono>
#include <regex>

using namespace std;

void clear_screen();


string get_current_date_time();

vector<std::string> split(const string& str);

string remove_quotes(string str);

string strip(const std::string &str);

void set_color(int color);

void cout_err(const string& msg);