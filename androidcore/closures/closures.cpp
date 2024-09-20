#include "closures.hpp"

#include <lapi.h>
#include <lstate.h>
#include <lgc.h>
#include <lfunc.h>
#include <lualib.h>

namespace cclosures {

    static void recursivelymarkproto(Proto* proto) {
        if (!proto) return;

        proto->linedefined = marked_proto;

        for (int i = 0; i < proto->sizep; ++i) {
            recursivelymarkproto(proto->p[i]);
        }
    }

    // Mark a Lua closure based on its type (C or Lua)
    static void markcclosure(Closure* closure) {
        if (!closure) return;

        if (closure->isC) {
            // Mark the closure as a C closure
            closure->isC = marked_c_closure;
        }
        else {
            // Recursively mark Lua prototypes
            recursivelymarkproto(closure->l.p);
        }
    }

    // Pushes a C closure onto the Lua stack and marks it for identification
    void pushcclosure(lua_State* L, lua_CFunction fn, const char* name, int num_upvalues, lua_Continuation k) {
        lua_pushcclosurek(L, fn, name, num_upvalues, k);
        Closure* closure = clvalue(luaA_toobject(L, -1));

        if (closure) {
            markcclosure(closure);
        }
    }

    // Determine the type of the closure, whether it's a marked C closure, a Lua closure, or something else
    types getcclosuretype(Closure* closure) {
        if (!closure) return unidentified;

        if (closure->isC) {
            switch (closure->isC) {
            case marked_c_closure:
                return marked_c_closure;
            case wrapped_l_closure:
                return wrapped_l_closure;
            default:
                return c_closure;
            }
        }
        else {
            // Check if the Lua closure's prototype has been marked
            return (closure->l.p->linedefined == marked_proto) ? marked_proto : l_closure;
        }

        return unidentified;
    }
}
