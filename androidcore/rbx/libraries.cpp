#include "libraries.hpp"
#include "globals/globals.hpp"
#include "globals/globalsfuncs.hpp"
#include "../logs.h"
#include "scripts/luaui.hpp"
#include "execution/script_execution.hpp"

#include <ltable.h>
#include <lstate.h>
#include <lualib.h>

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

    this->rL = lua_newthread(aL);
   // this->rL_ref = main; UNUSED

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
