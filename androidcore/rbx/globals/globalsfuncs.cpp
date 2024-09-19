#include "globalsfuncs.hpp"
#include "../../logs.h"
#include "globals.hpp"
#include <lua.h>
#include <string>
#include <map>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
#include <lstate.h>
#include "../execution/script_execution.hpp"
#include "../../memory/memory.hpp"
#include "../libraries.hpp"

auto globals::funcs::initiate() -> void
{
	CALLLOG();
	using namespace addresses;
	rbx_get_state = reinterpret_cast<decltype(rbx_get_state)>(memory::libraries::getaddr("libroblox.so", rbx_main_state));
	rbx_std_out_t = reinterpret_cast<decltype(rbx_std_out_t)>(memory::libraries::getaddr("libroblox.so", rbx_std_out));
	rbx_spawn_t = reinterpret_cast<decltype(rbx_spawn_t)>(memory::libraries::getaddr("libroblox.so", rbx_spawn));
	firetouch_interest_t = reinterpret_cast<decltype(firetouch_interest_t)>(memory::libraries::getaddr("libroblox.so", firetouch_interest));
	scriptcontext_rsm_t = reinterpret_cast<decltype(scriptcontext_rsm_t)>(memory::libraries::getaddr("libroblox.so", scriptcontext_rsm));
	rbx_luauload_t = reinterpret_cast<decltype(rbx_luauload_t)>(memory::libraries::getaddr("libroblox.so", rbx_luauload));
	set_thread_context_t = reinterpret_cast<decltype(set_thread_context_t)>(memory::libraries::getaddr("libroblox.so", set_thread_context));
	get_thread_context_t = reinterpret_cast<decltype(get_thread_context_t)>(memory::libraries::getaddr("libroblox.so", get_thread_context));
}

auto globals::funcs::decryptrbxstate(int64_t scriptcontext) -> lua_State*
{
	CALLLOG();
	int identity = 0;

	int64_t script = 0;

	const auto ls = rbx_get_state(scriptcontext, &identity, &script);
	return ls;
}

auto globals::funcs::setidentity(lua_State* ls, int identity) -> void
{
	auto extraspace = reinterpret_cast<ExtraSpace_t*>(lua_getthreaddata(ls));
	if (extraspace)
	{
		extraspace->context_level = identity;
	}

	auto context = get_thread_context_t(ls);
	if (context)
	{
		*reinterpret_cast<int64_t*>(context) = identity;
	}
}

auto globals::funcs::getidentity(lua_State* ls) -> int
{
	auto extraspace = reinterpret_cast<ExtraSpace_t*>(lua_getthreaddata(ls));
	if (extraspace)
	{
		return extraspace->context_level;
	}

	return 0;
}