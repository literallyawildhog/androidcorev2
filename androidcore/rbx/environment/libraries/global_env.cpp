#include "../environment.hpp"
#include "../../globals/globalsfuncs.hpp"
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>
#include "../../execution/script_execution.hpp"

// Retrieves the global environment (getgenv)
static int getgenv(lua_State* ls) {
    CALLLOG();
    lua_pushvalue(ls, LUA_GLOBALSINDEX);
    return 1;
}

// Retrieves the registry (getreg)
static int getreg(lua_State* ls) {
    CALLLOG();
    lua_pushvalue(ls, LUA_REGISTRYINDEX);
    return 1;
}

// Sets the global environment (setgenv)
static int setgenv(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    lua_replace(ls, LUA_GLOBALSINDEX);
    return 0;
}

// Get global value (getglobal)
static int getglobal(lua_State* ls) {
    CALLLOG();
    const char* varname = luaL_checkstring(ls, 1);
    lua_getglobal(ls, varname);
    return 1;
}

// Set global value (setglobal)
static int setglobal(lua_State* ls) {
    CALLLOG();
    const char* varname = luaL_checkstring(ls, 1);
    luaL_checkany(ls, 2);
    lua_setglobal(ls, varname);
    return 0;
}

// Check if the rbx is active (isrbxactive)
static int isrbxactive(lua_State* ls) {
    CALLLOG();
    lua_pushboolean(ls, true);
    return 1;
}

// idk what to say in this
static int isLuau(lua_State* ls) {
    CALLLOG();
    lua_pushboolean(ls, true);
    return 1;
}

static int pushscript(lua_State* ls) {
    if (!lua_isstring(ls, 1)) {
        luaL_error(ls, "Expected a string argument for the script");
        return 0;
    }
    const char* script = luaL_checkstring(ls, 1);
    script::exec::get_singleton()->execute(script);

    return 0;
}


auto register_global_env_functions(lua_State* ls) {
    using namespace globals::environment;
    make_lib(ls, nullptr, [=]() {
        add_member(ls, getgenv, "getgenv");
        add_member(ls, getreg, "getreg");
        add_member(ls, setgenv, "setgenv");
        add_member(ls, getglobal, "getglobal");
        add_member(ls, setglobal, "setglobal");
        add_member(ls, isLuau, "isLuau");
        add_member(ls, isrbxactive, "isrbxactive");
        add_member(ls, pushscript, "pushscript");
        });
}

