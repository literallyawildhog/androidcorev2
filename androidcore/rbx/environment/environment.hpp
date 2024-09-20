#pragma once

#include <lua.h>
#include <lualib.h>
#include <functional>
#include <variant>
#include "../../closures/closures.hpp"
#include "libraries/registry.hpp"
#include <list>

namespace globals::environment {
    class c_registry {
    private:
        bool registered;

    public:
        auto setup() -> void;
        auto is_registered() -> bool { return this->registered; }

        auto register_libraries(lua_State* ls) -> void;
    };

    auto make_alias(lua_State* ls, const char* key, std::list< const char* > aliases, bool is_alias_global = true, bool is_key_global = true) -> void;
    auto make_lib(lua_State* ls, const char* name, const std::function<void()>& initializer, bool is_global = true) -> void;

    // register member to the table at the top of the stack
    template <lua_Type T = LUA_TFUNCTION> // we mostly register functions
    auto add_member(lua_State* ls, std::variant<lua_CFunction, int> member, const char* name) -> void
    {
        if (!lua_istable(ls, -1))
        {
            luaL_error(ls, "%s", "Table not found at the top of the stack");
        }

        bool was_readonly{ false };
        if (lua_getreadonly(ls, -1))
        {
            lua_setreadonly(ls, -1, false);
            was_readonly = true;
        }

        switch (T)
        {
        case LUA_TNUMBER:
            lua_pushnumber(ls, std::get< int >(member));
            break;

        case LUA_TFUNCTION:
            cclosures::pushcclosure(ls, std::get< lua_CFunction >(member), name, 0, nullptr);
            break;

        default:
            luaL_error(ls, "Unsupported type %i", T);
        }

        lua_setfield(ls, -2, name);

        if (was_readonly) lua_setreadonly(ls, -1, true);
    }
}