#include "script_execution.hpp"
#include "../globals/globalsfuncs.hpp"
#include "../globals/globals.hpp"
#include "../../logs.h"
#include <thread>
#include <mutex>

std::mutex queue_mutex;

// Executes a Lua script
auto script::exec::execute(const std::string& script) -> void
{
    if (script.empty()) {
        LOGE("Attempted to execute an empty script.");
        return;
    }

    auto bytecode = Luau::compile(script, {}, {}, nullptr); // No encoder
}

// Schedules a yield for a thread
auto script::exec::schedule_yield(std::pair<globals::outputs::live_thread_ref*, int> yield) -> void
{
    std::lock_guard<std::mutex> lock(queue_mutex);
}

// Yields a Lua function
auto script::exec::yield_function(lua_State* ls, const std::function<yield_t()>& yielding_closure) -> int
{
    lua_pushthread(ls); // Push the current thread onto the stack

    std::thread([=]() mutable {
        try {
            auto retfunc = yielding_closure();
            retfunc(ls); // Execute the yielding function

            // Store the Lua state reference
            globals::outputs::live_thread_ref* ref = new globals::outputs::live_thread_ref;
            ref->th = ls;
            lua_pushthread(ls);
            ref->thread_id = lua_ref(ls, LUA_REGISTRYINDEX); // Store thread reference
            lua_pop(ls, 1);

            schedule_yield(std::make_pair(ref, -1)); // Schedule the yield
        }
        catch (const std::exception& ex) {
            LOGE("Exception in yield_function thread: %s", ex.what());
        }
        catch (...) {
            LOGE("Unknown exception in yield_function thread.");
        }
        }).detach(); // Detach the thread for asynchronous execution

    ls->base = ls->top; // Prepare Lua state for yielding
    ls->status = LUA_YIELD;
    ls->ci->flags |= 1; // Mark the current call info as yielding

    return -1; // Indicate that the function has yielded
}
