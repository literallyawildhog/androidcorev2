#pragma once

#include <cstdint>
#include <lua.h>
#include "globals.hpp"
#include <memory>

using namespace globals::outputs;

namespace globals::funcs {
	struct ExtraSpace_t;
	auto initiate() -> void;
	auto decryptrbxstate(std::int64_t scriptcontext) -> lua_State*;
    auto setidentity(lua_State* ls, int identity) -> void;
    auto getidentity(lua_State* ls) -> int;

	inline lua_State* (*rbx_get_state)(std::int64_t sc, int* identity_info, int64_t* thread_script);
	inline void (*rbx_std_out_t)(print_types type, const char* fmt, ...);
    inline int64_t(*get_thread_context_t)(lua_State* thread);
	inline int (*rbx_spawn_t)(lua_State* aL);
	inline int64_t(*firetouch_interest_t)(int64_t detector, float distance, int64_t* player);
	inline int (*scriptcontext_rsm_t)(int unk, int64_t sc, int64_t* ref, int nargs, int, int);
	inline int (*rbx_luauload_t)(lua_State* aL, const char* chunkname, const char* data, size_t size, int env);
	inline int (*set_thread_context_t)(lua_State* aL);
    
    struct weak_thread_ref_t
    {
        std::uint8_t pad_0[8];

        weak_thread_ref_t* previous; 
        weak_thread_ref_t* next;
        live_thread_ref* livethreadref; 
    };

    struct Node_t
    {
        std::uint8_t pad_0[4];

        weak_thread_ref_t* wtr;
    };

    struct ExtraSpace_t
    {
        struct Shared {
            int threadCount;
            std::uintptr_t script_context;

            std::uint8_t pad_0[12];

            ExtraSpace_t* allThreads;
        };

        ExtraSpace_t* previous;
        size_t count;
        ExtraSpace_t* next;
        std::shared_ptr<Shared> shared;

        Node_t* node;
        int context_level;

        std::uint8_t pad_0[12];

        std::uintptr_t script_context;
        std::uintptr_t unk_0;
        std::uintptr_t context_capability_mask;
        std::uintptr_t unk_1;
        std::uintptr_t script;

        void* script_ref_1;
        void* script_ref_2;
        void* script_ref_3;
        std::uintptr_t script_parent;
        std::uintptr_t script_parent_ref;
    };
}