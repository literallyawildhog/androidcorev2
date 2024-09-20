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
        lua_State* aL = nullptr;
        lua_State* rL = nullptr;
        int rL_ref = LUA_NOREF; 

        int64_t rL_u = 0;  
        bool isactive = false;

    public:
        auto getactive() const -> bool { return isactive; }

        auto get_aL_state() const -> lua_State* { return aL; }
        auto get_rL_state() const -> lua_State* { return rL; }
        auto get_rL_state_u() const -> int64_t { return rL_u; }

        auto initiate(std::int64_t scriptcontext) -> void;
        auto exit() -> void;
    };

}
