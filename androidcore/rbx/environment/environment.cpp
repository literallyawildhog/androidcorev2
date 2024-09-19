#pragma once
#include "environment.hpp"
#include "libraries/registry.hpp"

#include <lgc.h>

auto globals::environment::c_registry::register_libraries(lua_State* ls) -> void
{
    //depend(ls);
}

auto globals::environment::make_alias(lua_State* ls, const char* key, std::list< const char* > aliases, bool is_alias_global, bool is_key_global) -> void
{
    for (const auto& alias : aliases)
    {
        if (is_key_global)
        {
            lua_getglobal(ls, key);
        }
        else
        {
            lua_getfield(ls, -1, key);
        }

        if (lua_isnoneornil(ls, -1))
        {
            lua_pop(ls, 1);
            return;
        }

        if (is_alias_global)
        {
            lua_setglobal(ls, alias);
        }
        else
        {
            bool was_readonly{ false };
            if (lua_getreadonly(ls, -2))
            {
                lua_setreadonly(ls, -2, false);
                was_readonly = true;
            }

            lua_setfield(ls, -2, alias);

            if (was_readonly) lua_setreadonly(ls, -1, true);
        }
    }
}

auto globals::environment::make_lib(lua_State* ls, const char* name, const std::function<void()>& initializer, bool is_global) -> void
{
    if (is_global)
    {
        lua_pushvalue(ls, LUA_GLOBALSINDEX);
    }

    if (name)
    {
        lua_newtable(ls);

        initializer();

        lua_setreadonly(ls, -1, true);
        lua_setfield(ls, -2, name);
    }
    else
    {
        initializer();
    }

    if (is_global)
    {
        lua_pop(ls, 1);
    }
}
