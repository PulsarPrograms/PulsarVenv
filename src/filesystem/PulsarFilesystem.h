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

    // * Функционал 
    static void handle_cd(const std::string path);
    static void handle_mkdir(const std::string path);
    static void handle_mkсd(const std::string path);
    static void handle_rmdir(const std::string path);
    static void handle_ls(const std::filesystem::path path = PulsarFilesystem::cur_path);
    static void handle_crfile(const std::filesystem::path path = PulsarFilesystem::cur_path);
    static void handle_morefile(const std::string path);
    static void handle_writefile(const std::string path, const std::string text);
    static void handle_crwfile(const std::filesystem::path path, const std::string text);
};
