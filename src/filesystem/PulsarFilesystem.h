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
    static std::filesystem::path cur_path;
    static void handle_cd(const std::string path);
    static void handle_mkdir(const std::string path);
    static void handle_mkсd(const std::string path);
    static void handle_rmdir(const std::string path);
    static void handle_ls();
};
