#include "rbx/module.hpp"
#include "logs.h"
#include "memory/memory.hpp"
#include "rbx/filesystem/filesystem.hpp"
#include <thread>

auto start() -> void
{
	LOGD("Waiting for libroblox.so");
	memory::libraries::waitforlib("libroblox.so");
	memory::libraries::waitforlib("libandroidcore.so"); // preventing anyone to rename your shared library

	file_system::create_module_dirs("/storage/emulated/0/Documents/");
	globals::module::initiate();
	LOGD("Starting Exploit..");
}

[[maybe_unused]] __attribute__((constructor))
auto load_lib() -> void
{
	std::thread{ start }.detach();
}