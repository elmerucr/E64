/*
 * main.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "common.hpp"
#include "lua.hpp"
#include "host.hpp"
#include "settings.hpp"
#include "sound.hpp"
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	std::chrono::time_point<std::chrono::steady_clock> refresh_moment;
	std::chrono::time_point<std::chrono::steady_clock> end_time;
	
	E64::host_t *host = new E64::host_t();
	E64::settings_t *settings = new E64::settings_t(host);
	E64::sound_ic *sound = new E64::sound_ic(host);
	
	// Lua test
	lua_State *L;
	L = luaL_newstate();
	if (!L) exit(1); else printf("[Lua] %s\n", LUA_COPYRIGHT);
	luaL_openlibs(L);
	char lua_code[] = R"Lua(
		function test()
			print ("\nHi!")
		end

		rt = 45.345
		print("result is " .. rt / 3)
		test()
	)Lua";
	luaL_dostring(L, lua_code);
	lua_close(L);
	// end Lua test
	
	/*
	 * Volume
	 */
	sound->write_byte(0x208, 0x7f);
	sound->write_byte(0x209, 0x7f);
	
	bool running = true;
	
	// temp hack
	int8_t teller = 40;
	
	// place this in a class
	uint32_t cycles_per_tick = SID_CLOCK_SPEED / 50;
	uint32_t tick_cycles_remaining = 0;
	
	start_time = refresh_moment = std::chrono::steady_clock::now();
	
	/*
	 * Frame loop
	 */
	while (running) {
		/*
		 * Start with measuring audio_buffer and determine cycles to run in audio
		 */
		double frame_cycles_remaining = host->get_queued_audio_size_bytes(); // contains buffer in bytes
		//printf("buffer in bytes: %i\n", (int32_t)frame_cycles_remaining); // print it
		frame_cycles_remaining = SID_CLOCK_SPEED * (AUDIO_BUFFER_SIZE - frame_cycles_remaining) / (host->get_bytes_per_ms() * 1000); // adjust to needed buffer size + change to cycles
		frame_cycles_remaining += SID_CLOCK_SPEED / FPS;
		
		/*
		 * Run needed frame cycles, keeping audio_callback ticks into account
		 */
		while (frame_cycles_remaining) {
			if (tick_cycles_remaining > frame_cycles_remaining) {
				tick_cycles_remaining -= frame_cycles_remaining;
				sound->run(frame_cycles_remaining);
				frame_cycles_remaining = 0;
			} else {
				frame_cycles_remaining -= tick_cycles_remaining;
				sound->run(tick_cycles_remaining);
				// do_callback
				if (!teller) {
					sound->write_byte(0x100, 0x03); // start kick
					teller += 29;
				} else {
					sound->write_byte(0x100, 0x02); // silence
				}
				teller--;
				// end of do_callback
				tick_cycles_remaining = cycles_per_tick;
			}
		}
		
		/*
		 * Process events
		 */
		SDL_Event my_event;
		while (SDL_PollEvent(&my_event)) {
			if (my_event.type == SDL_QUIT) running = false;
		}
		
		/*
		 * If vsync is enabled, the update screen function takes more
		 * time, i.e. it will return after a few milliseconds, exactly
		 * when vertical refresh is done. This will avoid tearing.
		 * There's no need then to let the system sleep with a
		 * calculated value. But we will still have to do a time
		 * measurement for estimation of idle time.
		 *
		 * When there's no vsync, sleep time is done manually.
		 */
		if (host->vsync_disabled()) {
			refresh_moment += std::chrono::microseconds(1000000/FPS);
			/*
			 * Check if the next update is in the past. If so,
			 * calculate a new update moment. This will avoid
			 * "playing catch-up" by the virtual machine.
			 */
			if (refresh_moment > std::chrono::steady_clock::now()) {
				std::this_thread::sleep_until(refresh_moment);
			} else {
				refresh_moment = std::chrono::steady_clock::now();
			}
		}
		
		host->update_screen();
	}
	
	end_time = std::chrono::steady_clock::now();
	
	delete sound;
	delete settings;
	delete host;
	
	return 0;
}
