#pragma once

#include <string>
#include <vector>
#include <tuple>

namespace file_system
{
    enum return_codes {
        Exception_thrown = -1,
        Ok = 0,
        No_Exist = 1,
        Cant_Open = 2,
        Invalid_path = 3,
        Blacklisted_ext = 4
    };

    std::string get_module_path();
    std::string get_workspace_path();
    std::string get_autoexec_path();
    std::string get_asset_path();

    void create_module_dirs(const std::string& root_path);

    return_codes is_valid_path(const std::string& path);
    bool is_directory(const std::string& path);
    bool is_regular_file(const std::string& path);

    std::tuple<std::string, return_codes> read_file(const std::string& file_path);
    return_codes write_file(const std::string& file_path, const std::string& content);
    return_codes append_file(const std::string& file_path, const std::string& content);
    return_codes copy_file(const std::string& file_path, const std::string& destination);

    std::vector<std::string> list_files(const std::string& path);

    void remove_file(const std::string& path);
    void remove_directory(const std::string& path);

    void create_directories(const std::string& path);
}