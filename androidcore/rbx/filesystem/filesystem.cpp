#include "filesystem.hpp"
#include "../../logs.h"

#include "../../thirdparty-includes/ghc/filesystem.hpp"
#include <fstream>
#include <set>

namespace fs = ghc::filesystem;

static fs::path module_path;
static fs::path workspace_path;
static fs::path autoexec_path;
static fs::path asset_path;

std::string file_system::get_module_path()
{
    return module_path;
}

std::string file_system::get_workspace_path()
{
    return workspace_path;
}

std::string file_system::get_autoexec_path()
{
    return autoexec_path;
}

std::string file_system::get_asset_path()
{
    return asset_path;
}

void file_system::create_module_dirs(const std::string& root_path)
{
    const auto storage_path = fs::path(root_path);

    module_path = storage_path / config::filename;
    fs::create_directories(module_path);

    workspace_path = module_path / "workspace/";
    fs::create_directories(workspace_path);

    autoexec_path = module_path / "scripts/";
    fs::create_directories(autoexec_path);
}

file_system::return_codes file_system::is_valid_path(const std::string& path)
{
    static const std::set<std::string> blacklisted = { ".exe", ".scr", ".bat", ".com", ".csh",".msi",".vb",".vbs",".vbe",".ws",".wsf",".wsh",".ps1",".py" };

    auto copy = std::string(path);

    std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c) { return std::tolower(c); });

    for (const auto& ext : blacklisted)
    {
        if (copy.find(ext) != std::string::npos)
        {
            return Blacklisted_ext;
        }
    }

    if (copy.find("..") != std::string::npos)
    {
        return Invalid_path;
    }

    return Ok;
}

bool file_system::is_directory(const std::string& path)
{
    return fs::is_directory(path);
}

bool file_system::is_regular_file(const std::string& path)
{
    return fs::is_regular_file(path);
}

std::tuple<std::string, file_system::return_codes> file_system::read_file(const std::string& file_path)
{
    if (is_regular_file(file_path) == false)
    {
        return { "", No_Exist };
    }

    std::ifstream file(file_path, std::ios::in | std::ios::binary);
    auto file_size = fs::file_size(file_path);

    if (!file.is_open())
    {
        return { "", Cant_Open };
    }

    std::string file_contents(file_size, '\0');
    file.read(file_contents.data(), file_size);
    file.close();

    return { file_contents, Ok };
}

file_system::return_codes file_system::write_file(const std::string& file_path, const std::string& content)
{
    std::ofstream out(file_path, std::ios::out | std::ios::binary);

    if (!out.is_open())
    {
        return Cant_Open;
    }

    out << content;
    out.close();

    return Ok;
}

file_system::return_codes file_system::append_file(const std::string& file_path, const std::string& content)
{
    std::ofstream out(file_path, std::ios::out | std::ios::binary | std::ios::app);

    if (!out.is_open())
    {
        return Cant_Open;
    }

    out << content;
    out.close();

    return Ok;
}

file_system::return_codes file_system::copy_file(const std::string& file_path, const std::string& destination)
{
    try
    {
        fs::copy_file(file_path, destination, fs::copy_options::update_existing);
    }
    catch (fs::filesystem_error& e)
    {
        LOGE(" Exception thrown during file_system::copy_file. Ex: %s", e.what());
        return Exception_thrown;
    }
    return Ok;
}

std::vector<std::string> file_system::list_files(const std::string& path)
{
    std::vector<std::string> file_list;

    if (!fs::exists(path))
    {
        return file_list;
    }

    for (const auto& file : fs::directory_iterator(path))
    {
        std::string full_path = file.path();

        if (full_path.compare(0, workspace_path.string().length(), workspace_path) == 0)
        {
            full_path = full_path.substr(workspace_path.string().length());
        }
        else if (full_path.compare(0, autoexec_path.string().length(), autoexec_path) == 0)
        {
            full_path = full_path.substr(autoexec_path.string().length());
        }

        file_list.emplace_back(full_path);
    }


    return file_list;
}

void file_system::remove_file(const std::string& path)
{
    fs::remove(path);
}

void file_system::remove_directory(const std::string& path)
{
    fs::remove_all(path);
}

void file_system::create_directories(const std::string& path)
{
    auto copy = std::string(path);
    std::string dirs;

    std::string delimiter = "/";
    std::string token;

    size_t pos = copy.find_last_of(delimiter);
    if (pos != std::string::npos)
    {
        dirs = copy.substr(0, pos);
    }

    fs::create_directories(dirs);
}