#include "../environment.hpp"
#include "../../globals/globalsfuncs.hpp"
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>



// Send a remote event (fire_remote_event)
static int fire_remote_event(lua_State* ls) {
    CALLLOG();
    const char* eventName = luaL_checkstring(ls, 1);
    lua_getfield(ls, LUA_GLOBALSINDEX, "game");
    lua_getfield(ls, -1, "ReplicatedStorage");
    lua_getfield(ls, -1, eventName);
    lua_call(ls, 0, 0); // Fire the event
    return 0;
}

// Send a remote function call (invoke_remote_function)
static int invoke_remote_function(lua_State* ls) {
    CALLLOG();
    const char* functionName = luaL_checkstring(ls, 1);
    lua_getfield(ls, LUA_GLOBALSINDEX, "game");
    lua_getfield(ls, -1, "ReplicatedStorage");
    lua_getfield(ls, -1, functionName);
    lua_pushvalue(ls, 2); // Push arguments if needed
    lua_call(ls, 1, 1); // Call the function and return the result
    return 1;
}

// Connect to a remote event (connect_remote_event)
static int connect_remote_event(lua_State* ls) {
    CALLLOG();
    const char* eventName = luaL_checkstring(ls, 1);
    lua_getfield(ls, LUA_GLOBALSINDEX, "game");
    lua_getfield(ls, -1, "ReplicatedStorage");
    lua_getfield(ls, -1, eventName);
    lua_pushvalue(ls, 2); // Push the callback function
    lua_call(ls, 1, 0); // Connect the event
    return 0;
}

// Disconnect a remote event (disconnect_remote_event)
static int disconnect_remote_event(lua_State* ls) {
    CALLLOG();
    const char* eventName = luaL_checkstring(ls, 1);
    lua_getfield(ls, LUA_GLOBALSINDEX, "game");
    lua_getfield(ls, -1, "ReplicatedStorage");
    lua_getfield(ls, -1, eventName);
    lua_pushvalue(ls, 2); // Push the callback function
    lua_call(ls, 1, 0); // Disconnect the event
    return 0;
}

auto register_network_functions(lua_State* ls) {
    using namespace globals::environment;
    make_lib(ls, nullptr, [=]() {
        add_member(ls, fire_remote_event, "fire_remote_event");
        add_member(ls, invoke_remote_function, "invoke_remote_function");
        add_member(ls, connect_remote_event, "connect_remote_event");
        add_member(ls, disconnect_remote_event, "disconnect_remote_event");
        });
}
