#pragma once
#include <string>

namespace memory::libraries {
	auto waitforlib(const char* const name) -> void;
	auto getaddr(const char* const name, std::int64_t addy) -> int64_t;
	auto islibloaded(const char* const name) -> bool;
}