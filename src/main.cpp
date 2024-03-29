/*
 * main.cpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#include "common.hpp"
#include "host.hpp"
#include "settings.hpp"
#include "sound.hpp"
#include "core.hpp"
#include "keyboard.hpp"
#include "stats.hpp"
#include "hud.hpp"
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::steady_clock> app_start_time = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> end_of_frame_time;
	
	printf("E64 Computer System v%i.%i.%i (C)2019-%i elmerucr\n",
	       E64_MAJOR_VERSION,
	       E64_MINOR_VERSION,
	       E64_BUILD, E64_YEAR);

	E64::settings_t *settings = new E64::settings_t();
	E64::host_t *host = new E64::host_t(settings);
	E64::keyboard_t *keyboard = new E64::keyboard_t(host);
	E64::sound_ic *sound = new E64::sound_ic(settings);
	E64::stats_t *stats = new E64::stats_t();
	E64::hud_t *hud = new E64::hud_t();
	host->set_hud(hud);
	
	E64::core_t *core = new E64::core_t(settings, host, keyboard, sound);
	
	bool running = true;
	
	end_of_frame_time = std::chrono::steady_clock::now();
	
	stats->reset();
	
	keyboard->reset();
	keyboard->enable_events();
	
	/*
	 * Frame loop
	 */
	while (running) {
		/*
		 * Audio: Measure audio_buffer and determine cycles to run
		 */
		uint32_t audio_buffer = host->get_queued_audio_size_bytes();
		stats->set_queued_audio_bytes(audio_buffer);
		// note that we need int32_t and not uint32_t!
		int32_t cycles = SID_CLOCK_SPEED * (AUDIO_BUFFER_SIZE - audio_buffer) / (host->get_bytes_per_ms() * 1000); // adjust to needed buffer size + change to cycles
		cycles += SID_CLOCK_SPEED / FPS;
		
		core->do_sound_and_timers(cycles);
		
		// Time measurement
		stats->start_core_time();
		
		/*
		 * Process events
		 */
		if (host->events_process_events() == E64::QUIT_EVENT) {
			running = false;
		}
		
		keyboard->process();
		
		/*
		 * Do core
		 */
		core->do_frame();
		
		/*
		 * Hud
		 */
		hud->print_stats(stats->summary());
		hud->redraw();
		
		// Time measurement
		stats->start_update_textures_time();
		
		host->update_textures(core->blitter, hud->blitter);
		
		// Time measurement
		stats->start_idle_time();
		
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
			end_of_frame_time += std::chrono::microseconds(1000000/FPS);
			/*
			 * If the next update is in the past, calculate a
			 * new update moment.
			 */
			if (end_of_frame_time > std::chrono::steady_clock::now()) {
				std::this_thread::sleep_until(end_of_frame_time);
			} else {
				end_of_frame_time = std::chrono::steady_clock::now();
			}
		}
		
		host->update_screen(core->blitter, hud->blitter);
		
		/*
		 * time measurement, starting sound time
		 *
		 * This point marks the start of a new frame, also at this very
		 * moment it's good to measure the soundbuffer size.
		 */
		stats->start_sound_time();
		
		stats->process_parameters();
	}
	
	printf("[E64] Virtual machine ran for %.2f seconds\n", (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - app_start_time).count() / 1000);
	
	delete hud;
	delete stats;
	delete core;
	delete sound;
	delete keyboard;
	delete settings;
	delete host;
	
	return 0;
}
