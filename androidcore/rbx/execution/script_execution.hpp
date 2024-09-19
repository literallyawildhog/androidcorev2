#pragma once

#include <queue>
#include "../globals/globals.hpp"
#include <Luau/Compiler.h>
#include <Luau/BytecodeBuilder.h>
#include <lstate.h>
#include <Luau/BytecodeUtils.h>
#include <mutex>
#include <functional>

namespace script {
    class bytecode_encoder_t : public Luau::BytecodeEncoder {
        inline void encode(uint32_t* data, size_t count) override
        {
            for (auto i = 0u; i < count;)
            {
                auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
                i += Luau::getOpLength(LuauOpcode(opcode));
                opcode *= 227;
            }
        }
    };

    class exec {
    public:
        static auto get_singleton() -> exec* {
            static exec* _thiz = nullptr;

            if (_thiz == nullptr)
                _thiz = new exec();

            return _thiz;
        }

        using yield_t = std::function<int(lua_State*)>;

        auto execute(const std::string& script) -> void;
        auto schedule_yield(std::pair<globals::outputs::live_thread_ref*, int> yield) -> void;
        auto yield_function(lua_State* ls, const std::function<yield_t()>& yielding_closure) -> int;
    };
}