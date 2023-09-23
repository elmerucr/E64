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
#include "blitter.hpp"
#include "stats.hpp"
#include "hud.hpp"
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::steady_clock> app_start_time = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> end_of_frame_time;
	
	printf("E64 version %i.%i.%i (C)2019-%i elmerucr\n",
	       E64_MAJOR_VERSION,
	       E64_MINOR_VERSION,
	       E64_BUILD, E64_YEAR);
	
	E64::blitter_ic *vm_blitter = new E64::blitter_ic(VM_MAX_PIXELS_PER_SCANLINE, VM_MAX_SCANLINES);
	E64::blitter_ic *hud_blitter = new E64::blitter_ic(VM_MAX_PIXELS_PER_SCANLINE, VM_MAX_SCANLINES);
	
	E64::settings_t *settings = new E64::settings_t();
	E64::host_t *host = new E64::host_t(settings);
	
	E64::sound_ic *sound = new E64::sound_ic(settings);
	E64::core_t *core = new E64::core_t(sound);
	E64::stats_t *stats = new E64::stats_t();
	E64::hud_t *hud = new E64::hud_t(hud_blitter);
	host->set_hud(hud);
	
	bool running = true;
	
	// place this in a future class
	uint32_t cycles_per_tick = SID_CLOCK_SPEED / 50;
	uint32_t tick_cycles_remaining = 0;
	
	end_of_frame_time = std::chrono::steady_clock::now();
	
	vm_blitter->set_clear_color(BLUE_03);
	vm_blitter->set_hor_border_size(16);
	vm_blitter->set_hor_border_color(BLUE_01);
	vm_blitter->set_ver_border_size(0x00);
	vm_blitter->set_ver_border_color(BLUE_01);
	
	//hud_blitter->set_ver_border_size(16);
	//hud_blitter->set_ver_border_color(AMBER_04);
	
	stats->reset();
	hud_blitter->reset();
	
	/*
	 * Frame loop
	 */
	while (running) {
		/*
		 * Audio: Measure audio_buffer and determine cycles to run
		 */
		double frame_cycles_remaining = host->get_queued_audio_size_bytes(); // contains buffer in bytes
		stats->set_queued_audio_bytes(frame_cycles_remaining); // store in stats
		frame_cycles_remaining = SID_CLOCK_SPEED * (AUDIO_BUFFER_SIZE - frame_cycles_remaining) / (host->get_bytes_per_ms() * 1000); // adjust to needed buffer size + change to cycles
		frame_cycles_remaining += SID_CLOCK_SPEED / FPS;
		
		/*
		 * Audio: Run cycles, keeping audio_callback ticks into account
		 */
		while (frame_cycles_remaining) {
			if (tick_cycles_remaining > frame_cycles_remaining) {
				tick_cycles_remaining -= frame_cycles_remaining;
				sound->run(frame_cycles_remaining, host);
				frame_cycles_remaining = 0;
			} else {
				frame_cycles_remaining -= tick_cycles_remaining;
				sound->run(tick_cycles_remaining, host);
				core->timer0_callback();
				tick_cycles_remaining = cycles_per_tick;
			}
		}
		
		float sample;
		if (settings->audio_recording) {
			while (settings->audio_record_buffer_pop(&sample)) {
				settings->write_to_wav(sample);
			}
		}
		
		stats->start_core_time();
		
		/*
		 * Process events
		 */
		if (host->events_process_events() == E64::QUIT_EVENT) running = false;
		
		/*
		 * Blitting vm
		 */
		vm_blitter->clear_framebuffer();
		vm_blitter->add_operation_draw_ver_border();
		vm_blitter->add_operation_draw_hor_border();
		while (vm_blitter->run_next_operation()) {}
		
		/*
		 * Blitting hud
		 */
		hud->print_stats(stats->summary());
		hud->redraw();
		
		// Time measurement
		stats->start_update_textures_time();
		
		host->update_textures(vm_blitter, hud_blitter);
		
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
		
		host->update_screen(vm_blitter);
		
		/*
		 * time measurement, starting core time
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
	delete settings;
	delete host;
	delete hud_blitter;
	delete vm_blitter;
	
	return 0;
}
