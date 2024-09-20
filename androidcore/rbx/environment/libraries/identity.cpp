#include "../environment.hpp"
#include "../../globals/globalsfuncs.hpp"
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>

// Sets the security identity (setidentity)
static int setidentity(lua_State* ls) {
    CALLLOG();
    auto identity = luaL_checkinteger(ls, 1);
    auto ES = static_cast<globals::funcs::ExtraSpace_t*>(lua_getthreaddata(ls));
    if (ES == nullptr) {
        luaL_argerror(ls, 0, "Could not get ExtraSpace, identity couldn't be set.");
        return 0;
    }
    lua_pushinteger(ls, ES->context_level);
    globals::funcs::setidentity(ls, identity);
    return 1;
}

// Gets the current security identity (getidentity)
static int getidentity(lua_State* ls) {
    CALLLOG();
    auto ES = static_cast<globals::funcs::ExtraSpace_t*>(lua_getthreaddata(ls));
    if (ES == nullptr) {
        luaL_argerror(ls, 0, "Could not get ExtraSpace, identity couldn't be retrieved.");
        return 0;
    }
    lua_pushinteger(ls, ES->context_level);
    return 1;
}

// Elevates the security identity (elevateidentity)
static int elevateidentity(lua_State* ls) {
    CALLLOG();
    auto ES = static_cast<globals::funcs::ExtraSpace_t*>(lua_getthreaddata(ls));
    if (ES == nullptr) {
        luaL_argerror(ls, 0, "Could not get ExtraSpace, identity couldn't be elevated.");
        return 0;
    }
    globals::funcs::setidentity(ls, 6); // Elevate to highest level
    lua_pushinteger(ls, 6);
    return 1;
}

// Identifies the executor (identifyexecutor)
static int identifyexecutor(lua_State* ls) {
    CALLLOG();
    lua_pushstring(ls, config::name);
    lua_pushstring(ls, config::version);
    return 2;
}

void register_identity_functions(lua_State* ls)
{
    using namespace globals::environment;

    make_lib(ls, nullptr, [=]() {
        add_member(ls, setidentity, "setidentity");
        add_member(ls, getidentity, "getidentity");
        add_member(ls, elevateidentity, "elevateidentity");
        add_member(ls, identifyexecutor, "identifyexecutor");

        make_alias(ls, "identifyexecutor", { "get_executor_name" });
        });
}