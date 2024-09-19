#pragma once

#include <cstdint>
#include <lua.h>
#include <string>
#include <map>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <lstate.h>
#include "execution/script_execution.hpp"

namespace globals {
	class libraries {
	private:
		lua_State* aL;
		lua_State* rL;
		int rL_ref;

		int64_t rL_u;

		bool isactive;
	public:
		auto getactive() -> bool { return this->isactive; }

		auto get_aL_state() -> lua_State* { return this->aL; }
		auto get_rL_state() -> lua_State* { return this->rL; }
		auto get_rL_state_u() -> int64_t { return this->rL_u; }

		auto initiate(std::int64_t scriptcontext) -> void;
		auto exit() -> void;
	};
}