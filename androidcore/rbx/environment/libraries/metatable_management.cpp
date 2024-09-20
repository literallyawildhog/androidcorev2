#include "../environment.hpp"
#include "../../globals/globalsfuncs.hpp"
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>

// Retrieves the metatable of a table (getmetatable)
static int getmetatable(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    lua_getmetatable(ls, -1);
    return 1; // Return metatable
}

// Sets the metatable of a table (setmetatable)
static int setmetatable(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    luaL_checktype(ls, 2, LUA_TTABLE);
    lua_setmetatable(ls, -2); // Set the metatable
    return 0; // No return value
}

// Checks if a table has a specific key in its metatable (metatable_haskey)
static int metatable_haskey(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    const char* key = luaL_checkstring(ls, 2);

    lua_getmetatable(ls, 1); // Push the metatable
    if (lua_isnil(ls, -1)) {
        lua_pop(ls, 1); // Remove nil
        lua_pushboolean(ls, 0); // No metatable
        return 1;
    }

    lua_getfield(ls, -1, key); // Get key from metatable
    int exists = !lua_isnil(ls, -1);
    lua_pop(ls, 2); // Pop the value and the metatable
    lua_pushboolean(ls, exists);
    return 1; // Return existence flag
}

// Sets a key in the metatable (metatable_setkey)
static int metatable_setkey(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    const char* key = luaL_checkstring(ls, 2);
    luaL_checkany(ls, 3);

    lua_getmetatable(ls, 1); // Get the metatable
    lua_pushvalue(ls, 2); // Push the key
    lua_pushvalue(ls, 3); // Push the value
    lua_settable(ls, -3); // Set the key-value pair in the metatable
    lua_pop(ls, 1); // Remove the metatable
    return 0; // No return value
}

// Retrieves a key from the metatable (metatable_getkey)
static int metatable_getkey(lua_State* ls) {
    CALLLOG();
    luaL_checktype(ls, 1, LUA_TTABLE);
    const char* key = luaL_checkstring(ls, 2);

    lua_getmetatable(ls, 1); // Push the metatable
    if (lua_isnil(ls, -1)) {
        lua_pop(ls, 1); // Remove nil
        lua_pushnil(ls); // No metatable
        return 1;
    }

    lua_getfield(ls, -1, key); // Get key from metatable
    return 1; // Return the value (or nil)
}

auto register_metatable_management_functions(lua_State* ls) {
    using namespace globals::environment;
    make_lib(ls, nullptr, [=]() {
        add_member(ls, getmetatable, "getmetatable");
        add_member(ls, setmetatable, "setmetatable");
        add_member(ls, metatable_haskey, "metatable_haskey");
        add_member(ls, metatable_setkey, "metatable_setkey");
        add_member(ls, metatable_getkey, "metatable_getkey");

        make_alias(ls, "metatable_setkey", { "set_meta_key" });
        });
}
