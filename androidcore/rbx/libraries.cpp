#include "libraries.hpp"
#include "globals/globals.hpp"
#include "globals/globalsfuncs.hpp"
#include "../logs.h"
#include "scripts/luaui.hpp"
#include "execution/script_execution.hpp"

#include <ltable.h>
#include <lstate.h>
#include <lualib.h>

auto globals::libraries::initiate(std::int64_t scriptcontext) -> void {
    CALLLOG();

    aL = globals::funcs::decryptrbxstate(scriptcontext);
    if (!aL) {
        LOGE("Failed to decrypt Roblox state");
        std::exit(EXIT_FAILURE); // Use std::exit for clarity
    }

    LOGI("Lua state (aL) initialized at %p", aL);

    // Activate Luau flags
    for (auto* flag = Luau::FValue<bool>::list; flag; flag = flag->next) {
        if (strncmp(flag->name, "Luau", 4) == 0) {
            LOGW("Activating Luau flag: %s", flag->name);
            flag->value = true;
        }
    }

    rL = lua_newthread(aL); // Create a new Lua thread
    luaL_sandboxthread(rL);  // Sandbox the thread

    // Set up global tables
    lua_newtable(rL);
    lua_setglobal(rL, "_G");
    lua_newtable(rL);
    lua_setglobal(rL, "shared");

    // Execute the Lua UI script
    auto* schedule = script::exec::get_singleton();
    schedule->execute(luaui);

    isactive = true;
    LOGI("Exploit environment started!");
}

auto globals::libraries::exit() -> void {
    CALLLOG();

    aL = nullptr;
    rL = nullptr;
    rL_u = 0;
    isactive = false;

    LOGD("Exploit environment exited and cleaned up!");
}
