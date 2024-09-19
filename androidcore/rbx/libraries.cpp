#include "libraries.hpp"
#include "globals/globals.hpp"
#include "globals/globalsfuncs.hpp"
#include "../logs.h"
#include "scripts/luaui.hpp"
#include "execution/script_execution.hpp"

#include <ltable.h>
#include <lstate.h>
#include <lualib.h>

auto createthread(lua_State* guardian) -> std::pair< lua_State*, int >
{
    CALLLOG();

    const auto old = lua_gettop(guardian);

    int identity;   
    const auto scid = lua_newthread(guardian);
    {
        identity = lua_ref(guardian, -1);
        scid->gt = luaH_clone(guardian, guardian->gt);
    }
    lua_pop(guardian, 1);

	globals::funcs::setidentity(scid, 9);

    lua_settop(guardian, old);

    return { scid, identity };
}

auto globals::libraries::initiate(std::int64_t scriptcontext) -> void
{
    CALLLOG();

    this->aL = globals::funcs::decryptrbxstate(scriptcontext);
    if (!aL) {
        LOGE("Failed to decrypt rbx state");
        exit();
        return;
    }

    LOGI("aL is %p", aL);

    for (Luau::FValue<bool>* flag = Luau::FValue<bool>::list; flag; flag = flag->next)
    {
        if (strncmp(flag->name, "Luau", 4) == 0)
        {
            LOGW("Luau flag is %s", flag->name);
            flag->value = true;
        }
    }

    const auto [sandbox, main] = createthread(aL);
    this->rL = sandbox;
    this->rL_ref = main;

    luaL_sandboxthread(sandbox);

    lua_newtable(sandbox);
    lua_setglobal(sandbox, "_G");

    lua_newtable(sandbox);
    lua_setglobal(sandbox, "shared");

    script::exec::get_singleton()->execute(luaui);

    isactive = true;
    LOGI("Exploit environment started!");
}

auto globals::libraries::exit() -> void
{
    CALLLOG();

    this->aL = nullptr;
    this->rL = nullptr;
    this->rL_u = 0;
    this->rL_u = 0;
    this->isactive = false;

    LOGD("Exploit environment killed!");
}
