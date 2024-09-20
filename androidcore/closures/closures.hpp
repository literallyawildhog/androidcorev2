#pragma once

#include <lua.h> 
#include <lstate.h>

namespace cclosures {
    enum types {
        c_closure,
        l_closure,
        marked_c_closure,
        wrapped_l_closure,
        marked_proto,
        unidentified
    };

    auto pushcclosure(lua_State* L, lua_CFunction fn, const char* name, int num_upvalues, lua_Continuation k) -> void;
    auto getcclosuretype(Closure* closure) -> types;

    void markcclosure(lua_State* L);
    void recursivelymarkproto(lua_State* L, Proto* proto);

}
