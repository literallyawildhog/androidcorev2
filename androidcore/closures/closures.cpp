#include "closures.hpp"

#include <lapi.h>
#include <lstate.h>
#include <lgc.h>
#include <lfunc.h>
#include <lualib.h>

auto cclosures::pushcclosure(lua_State* ls, lua_CFunction f, const char* name, int nups, lua_Continuation k) -> void
{
    lua_pushcclosurek(ls, f, name, nups, k);
    const auto cl = clvalue(luaA_toobject(ls, -1));

    markcclosure(cl);
}

auto mark_protos(Proto* p) -> void
{
    p->linedefined = cclosures::marked_proto;

    for (auto i = 0; i < p->sizep; i++)
    {
        mark_protos(p->p[i]);
    }
}

auto cclosures::markcclosure(Closure* cl) -> void
{
    if (cl->isC)
    {
        cl->isC = marked_c_closure;
    }
    else
    {
        const auto p = cl->l.p;

        mark_protos(p);
    }
}

auto cclosures::getcclosuretype(Closure* cl) -> types
{
    if (cl->isC)
    {
        if (cl->isC == marked_c_closure)
        {
            return marked_c_closure;
        }

        if (cl->isC == wrapped_l_closure)
        {
            return wrapped_l_closure;
        }

        return c_closure;
    }
    else if (!cl->isC)
    {
        if (cl->l.p->linedefined == marked_proto)
        {
            return marked_proto;
        }

        return l_closure;
    }

    return unidentified;
}