#pragma once

#include <lua.h>
#include <lobject.h>

namespace cclosures {
    enum types {
        unidentified = -2,
        marked_proto = -1,

        l_closure = 0,
        c_closure = 1,

        marked_c_closure = 2,
        wrapped_l_closure = 3, 
    };

    auto pushcclosure(lua_State* ls, lua_CFunction f, const char* name, int nups, lua_Continuation k) -> void;
    auto markcclosure(Closure* cl) -> void;
    auto clonecclosure(lua_State* ls, Closure* src) -> bool;
    auto getcclosuretype(Closure* cl) -> types;
}