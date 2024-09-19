#include "memory.hpp"
#include <thread>
#include <cstdio>
#include "../logs.h"
#include <string>
#include <fstream>
#include <sstream>

auto memory::libraries::getaddr(const char* const name, std::int64_t addy) -> int64_t
{
	auto base = std::int64_t{ 0 };

	if (!std::ifstream("/proc/self/maps")) {
		LOGE("Failed to open /proc/self/maps");
	}

	std::string line;
	while (std::getline(std::ifstream("/proc/self/maps"), line)) {
		if (line.find(name) != std::string::npos) {
			base = strtoul(line.c_str(), nullptr, 16);
			break;
		}
	}

	return base;
}

auto memory::libraries::islibloaded(const char* const name) -> bool
{
	LOGD("Calling islibloaded function.");

	std::ifstream maps("/proc/self/maps");

	if (!maps.is_open()) {
		LOGE("Failed to open /proc/self/maps");
		return false;
	}

	bool found{ false };
	for (std::string line; std::getline(maps, line);) {
		LOGD("Line is %s", line.c_str());

		if (line.find(name) != std::string::npos) {
			found = true;
			break;
		}
	}

	maps.close();

	return found;
}

auto memory::libraries::waitforlib(const char* const name) -> void
{
	LOGD("Calling waitforlib function.");
	while (!islibloaded(name))
	{
		LOGD("Library '{}' not loaded yet", name);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}