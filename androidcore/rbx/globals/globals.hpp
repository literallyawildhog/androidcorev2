#pragma once
#include <cstdint>
#include <lstate.h>

namespace globals {
	namespace addresses {
		constexpr int64_t rbx_main_state = 0x27C5E58;

		constexpr int64_t rbx_std_out = 0x3812964;
		constexpr int64_t rbx_luauload = 0x4E64718;
		constexpr int64_t rbx_spawn = 0x2809DC0;
		constexpr int64_t rbx_jobstop = 0x5C2CA08;
		constexpr int64_t firetouch_interest = 0x362F06C;
		constexpr int64_t scriptcontext_rsm = 0x27D3300;
		constexpr int64_t set_thread_context = 0x27C16EC;
		constexpr int64_t get_thread_context = 0x5A2112C;
	}

	namespace outputs {
		struct live_thread_ref
		{
			int unk_0;
			lua_State* th;
			int thread_id; 
		};

		enum print_types : uint8_t
		{
			print,
			info,
			warn,
			error
		};

		constexpr int scriptcontext_txt = 1280; // unused
	}
}