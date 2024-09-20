#pragma once

#include <lua.h>
#include <lualib.h>

#include "global_env.cpp"
#include "thread_management.cpp"
#include "identity.cpp"

auto register_global_env_functions(lua_State* ls);
//auto register_thread_management_functions(lua_State * ls);
//auto register_identity_functions(lua_State * ls);
//auto register_metatable_management_functions(lua_State* ls);
auto register_network_functions(lua_State* ls);