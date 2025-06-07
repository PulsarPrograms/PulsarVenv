#pragma once
#include <filesystem>
#include <string>
#include <vector>

class CommandFilesystem {
    public:
    int execute(std::vector<std::string> &command);
};

class PulsarFilesystem {
public:

    static std::filesystem::path cur_path;  // * Текущий путь
    static std::filesystem::path home_path; // * Home dir
    static std::filesystem::path last_path; // * last dir 

    // * Функционал 
    static void handle_cd(const std::string path);
    static void handle_cd();
    static void handle_mkdir(const std::string path);
    static void handle_mkсd(const std::string path);
    static void handle_rmdir(const std::string path);
    static void handle_ls(const std::filesystem::path path = PulsarFilesystem::cur_path);
    static void handle_crfile(const std::filesystem::path path = PulsarFilesystem::cur_path);
    static void handle_morefile(const std::string path);
    static void handle_writefile(const std::string path, const std::string text);
    static void handle_crwfile(const std::filesystem::path path, const std::string text);
    static void handle_rm(const std::filesystem::path path);
    static void handle_tree();
    static void handle_sys_tree(const std::filesystem::path path); // * Cистемный метод для tree
    static void handle_fsdown(int pos=1);
    static void handle_betcd(const std::string &fdir);
    static bool find_and_change_dir(const std::filesystem::path& search_path, const std::string& target_dir);
};
