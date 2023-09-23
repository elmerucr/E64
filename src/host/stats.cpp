/*
 * stats.cpp
 * E64
 *
 * Copyright © 2020-2023 elmerucr. All rights reserved.
 */

#include <cstdint>
#include <chrono>
#include <thread>
#include <cstdint>
#include <iostream>
#include "stats.hpp"
#include "common.hpp"

void E64::stats_t::reset()
{
	total_time = 0;
	total_sound_time = 0;
	total_core_time = 0;
	total_textures_time = 0;
	total_idle_time = 0;
	
	framecounter = 0;
	framecounter_interval = 4;
	
	status_bar_framecounter = 0;
	status_bar_framecounter_interval = FPS / 2;

	audio_queue_size_bytes = 0;
	smoothed_audio_queue_size_bytes = 0;
	
	smoothed_framerate = FPS;
	
	smoothed_sound_per_frame = 1000000 / (FPS * 4);
	smoothed_core_per_frame = 1000000 / (FPS * 4);
	smoothed_textures_per_frame = 1000000 / (FPS * 4);
	smoothed_idle_per_frame = 1000000 / (FPS * 4);
	
	cpu_percentage = 100 * (smoothed_sound_per_frame + smoothed_core_per_frame + smoothed_textures_per_frame) / (1000000 / FPS);
    
	alpha = 0.90f;
	alpha_cpu = 0.50f;
	
	frametime = 1000000 / FPS;

	start_sound = start_sound_old = std::chrono::steady_clock::now();
}

void E64::stats_t::process_parameters()
{
	framecounter++;
	
	if (framecounter == framecounter_interval) {
		framecounter = 0;

		framerate = (double)(framecounter_interval * 1000000) / total_time;
		
		smoothed_framerate =
			(alpha * smoothed_framerate) +
			((1.0 - alpha) * framerate);
        
		sound_per_frame = total_sound_time / framecounter_interval;
		core_per_frame = total_core_time / framecounter_interval;
		textures_per_frame = total_textures_time / framecounter_interval;
		idle_per_frame = total_idle_time / framecounter_interval;
		
		smoothed_sound_per_frame =
			(alpha * smoothed_sound_per_frame) +
			((1.0 - alpha) * sound_per_frame);
		
		smoothed_core_per_frame =
			(alpha * smoothed_core_per_frame) +
			((1.0 - alpha) * core_per_frame);
		
		smoothed_textures_per_frame =
			(alpha * smoothed_textures_per_frame) +
			((1.0 - alpha) * textures_per_frame);
		
		smoothed_idle_per_frame =
			(alpha * smoothed_idle_per_frame) +
			((1.0 - alpha) * idle_per_frame);
		
		smoothed_audio_queue_size_bytes =
			(alpha * smoothed_audio_queue_size_bytes) +
			((1.0 - alpha) * audio_queue_size_bytes);
		
		cpu_percentage = 100 * (smoothed_sound_per_frame + smoothed_core_per_frame + smoothed_textures_per_frame) / (smoothed_sound_per_frame + smoothed_core_per_frame + smoothed_textures_per_frame + smoothed_idle_per_frame);
        
		total_time = total_sound_time = total_core_time = total_textures_time = total_idle_time = 0;
	}

	status_bar_framecounter++;

	if (status_bar_framecounter == status_bar_framecounter_interval) {
		status_bar_framecounter = 0;

		snprintf(statistics_string, 256,
			"\n      sound: %5.2f ms     cpu load:%6.2f %%\n"
			"       core: %5.2f ms  audiobuffer: %5.2f kb\n"
			"   textures: %5.2f ms    framerate:%6.2f fps\n"
			"  frametime: %5.2f ms",
			smoothed_sound_per_frame/1000, cpu_percentage,
			smoothed_core_per_frame/1000, smoothed_audio_queue_size_bytes/1024,
			 smoothed_textures_per_frame/1000, smoothed_framerate,
			 (smoothed_sound_per_frame+smoothed_core_per_frame+smoothed_textures_per_frame+smoothed_idle_per_frame)/1000);
	}
}
