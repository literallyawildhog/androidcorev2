#include "../environment.hpp"
#include "../../globals/globalsfuncs.hpp"
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>

// Retrieves the thread environment (gettenv)
static int gettenv(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTHREAD);
    auto thread = lua_tothread(ls, 1);
    lua_pushvalue(thread, LUA_GLOBALSINDEX);
    lua_xmove(thread, ls, 1);
    return 1;
}

// Returns the current thread (getthread)
static int getthread(lua_State* ls) {
    CALLLOG();
    lua_pushthread(ls);
    return 1;
}

// Creates a new Lua thread (createthread)
static int createthread(lua_State* ls) {
    CALLLOG();
    lua_State* newthread = lua_newthread(ls);
    lua_pushvalue(ls, -1);
    return 1;
}

// Kills a given thread (killthread)
static int killthread(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTHREAD);
    auto thread = lua_tothread(ls, 1);
    lua_pushnil(thread);
    lua_close(thread);
    return 0;
}

void register_thread_management_functions(lua_State* ls) {
    using namespace globals::environment;
    make_lib(ls, nullptr, [=]() {
        add_member(ls, gettenv, "gettenv");
        add_member(ls, getthread, "getthread");
        add_member(ls, createthread, "createthread");
        add_member(ls, killthread, "killthread");
        });
}