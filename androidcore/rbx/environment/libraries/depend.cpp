#include "../environment.hpp"
#include "registry.hpp"
#include "../../globals/globalsfuncs.hpp"

#include <lua.h>
#include <lualib.h>
#include <string>
#include <lmem.h>
#include <lgc.h>
#include <../../../logs.h>

static int getgenv(lua_State* ls)
{
    CALLLOG();

    lua_pushvalue(ls, LUA_GLOBALSINDEX);
    return 1;
}

static int identifyexecutor(lua_State* ls)
{
    CALLLOG();
    lua_pushstring(ls, config::name);
    lua_pushstring(ls, config::version);
}

static int getreg(lua_State* ls)
{
    CALLLOG();

    lua_pushvalue(ls, LUA_REGISTRYINDEX);
    return 1;
}

static int gettenv(lua_State* ls)
{
    CALLLOG();

    luaL_checktype(ls, 1, LUA_TTHREAD);

    auto thread = lua_tothread(ls, 1);

    lua_pushvalue(thread, LUA_GLOBALSINDEX);
    lua_xmove(thread, ls, 1);
    return 1;
}

static int getthread(lua_State* ls)
{
    CALLLOG();

    lua_pushthread(ls);
    return 1;
}

static int setidentity(lua_State* ls)
{
    CALLLOG();

    auto identity = luaL_checkinteger(ls, 1);

    auto ES = static_cast<globals::funcs::ExtraSpace_t*>(lua_getthreaddata(ls));
    if (ES == nullptr)
    {
        luaL_argerror(ls, 0, "Could not get ExtraSpace, identity couldnt be set.");
        return 0;
    }

    lua_pushinteger(ls, ES->context_level);
    globals::funcs::setidentity(ls, identity);
    return 1;
}

/*
auto threads(lua_State* ls) -> void
{
    using namespace globals::environment;
    registerlib(
        ls,
        nullptr,
        [=]() {
            registerlibfunc(ls, getgenv, "getgenv");
            registerlibfunc(ls, getreg, "getreg");
            registerlibfunc(ls, gettenv, "gettenv");
            registerlibfunc(ls, getthread, "getthread");
            registerlibfunc(ls, setidentity, "setidentity");
            registerlibfunc(ls, setidentity, "set_identity");
            registerlibfunc(ls, identifyexecutor, "identifyexecutor");
            registerlibfunc(ls, identifyexecutor, "getexecutorname");
            // registerlibfunc(ls, schedule_script, "schedule_script");
        },
        true
    );
}
*/

auto funcs(lua_State* ls) -> void
{
    using namespace globals::environment;

    make_lib(ls, nullptr, [=]() {
        add_member(ls, getgenv, "getgenv");
        add_member(ls, getreg, "getreg");
        add_member(ls, gettenv, "gettenv");
        add_member(ls, getthread, "getthread");
        add_member(ls, setidentity, "setidentity");
        add_member(ls, setidentity, "set_identity");
        add_member(ls, identifyexecutor, "identifyexecutor");
        make_alias(ls, "identifyexecutor", { "getexecutorname" });
        });
}
