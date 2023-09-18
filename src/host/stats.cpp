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
	total_core_time = 0;
	total_textures_time = 0;
	total_idle_time = 0;
	
	framecounter = 0;
	framecounter_interval = 4;
	
	status_bar_framecounter = 0;
	status_bar_framecounter_interval = FPS / 2;

	audio_queue_size_bytes = 0;
	
	smoothed_framerate = FPS;
	
	smoothed_core_per_frame = 1000000 / (FPS * 3);
	smoothed_textures_per_frame = 1000000 / (FPS * 3);
	smoothed_idle_per_frame = 1000000 / (FPS * 3);
	
	cpu_percentage = 100 * (smoothed_core_per_frame + smoothed_textures_per_frame) / (1000000 / FPS);
    
	alpha = 0.90f;
	alpha_cpu = 0.50f;
	
	frametime = 1000000 / FPS;

	start_core = start_core_old = std::chrono::steady_clock::now();
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
        
		vm_per_frame = total_core_time / framecounter_interval;
		textures_per_frame = total_textures_time / framecounter_interval;
		idle_per_frame = total_idle_time / framecounter_interval;
		
		smoothed_core_per_frame =
			(alpha * smoothed_core_per_frame) +
			((1.0 - alpha) * vm_per_frame);
		
		smoothed_textures_per_frame =
			(alpha * smoothed_textures_per_frame) +
			((1.0 - alpha) * textures_per_frame);
		
		smoothed_idle_per_frame =
			(alpha * smoothed_idle_per_frame) +
			((1.0 - alpha) * idle_per_frame);
		
		cpu_percentage = 100 * (smoothed_core_per_frame + smoothed_textures_per_frame) / (smoothed_core_per_frame + smoothed_textures_per_frame + smoothed_idle_per_frame);
        
		total_time = total_core_time = total_textures_time = total_idle_time = 0;
	}

	status_bar_framecounter++;
	
	if (status_bar_framecounter == status_bar_framecounter_interval) {
		status_bar_framecounter = 0;
		
		snprintf(statistics_string, 256, "\n                          snd+core: %5.2f ms\n"
						 "   framerate: %6.2f fps  textures: %5.2f ms\n"
						 " soundbuffer: %6.2f kb       idle: %5.2f ms\n"
						 "    host cpu: %6.2f %%       total: %5.2f ms",
						 smoothed_core_per_frame/1000,
						 smoothed_framerate, smoothed_textures_per_frame/1000,
						 audio_queue_size_bytes/1024, smoothed_idle_per_frame/1000,
						 cpu_percentage,
						 (smoothed_core_per_frame+smoothed_textures_per_frame+smoothed_idle_per_frame)/1000);
	}
}
