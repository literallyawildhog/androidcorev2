#include "script_execution.hpp"
#include "../globals/globalsfuncs.hpp"
#include "../globals/globals.hpp"
#include "../../logs.h"
#include <thread>

std::mutex queue_mutex;

auto script::exec::execute(const std::string& script) -> void
{
    if (script.empty()) return;

    static auto encoder = bytecode_encoder_t();
    auto bytecode = Luau::compile(script, { }, { }, &encoder);
}

auto script::exec::schedule_yield(std::pair<globals::outputs::live_thread_ref*, int> yield) -> void
{
    std::lock_guard<std::mutex> lock(queue_mutex);
}

auto script::exec::yield_function(lua_State* ls, const std::function<yield_t()>& yielding_closure) -> int
{
    lua_pushthread(ls);
    lua_ref(ls, -1);
    lua_pop(ls, 1);

    std::thread([=]
        {
            try {
                auto retfunc = yielding_closure();
                retfunc(ls);

                globals::outputs::live_thread_ref* ref = new globals::outputs::live_thread_ref;
                ref->th = ls;
                lua_pushthread(ls);
                ref->thread_id = lua_ref(ls, -1);
                lua_pop(ls, 1);

                schedule_yield(std::make_pair(ref, -1));
            }
            catch (std::exception& ex) {
                LOGE("Exception thrown: %s", ex.what());
            }
        }).detach();

    ls->base = ls->top;
    ls->status = LUA_YIELD;

    ls->ci->flags |= 1;

    return -1;
}