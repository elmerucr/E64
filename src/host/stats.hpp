/*
 * stats.hpp
 * E64
 *
 * Copyright © 2020-2023 elmerucr. All rights reserved.
 */
 
#include <cstdint>
#include <chrono>
#include "host.hpp"

#ifndef STATS_HPP
#define STATS_HPP

namespace E64
{

class stats_t
{
private:
	E64::host_t *host;
	
	std::chrono::time_point<std::chrono::steady_clock> start_vm;
	std::chrono::time_point<std::chrono::steady_clock> start_vm_old;
	std::chrono::time_point<std::chrono::steady_clock> start_update_textures;
	std::chrono::time_point<std::chrono::steady_clock> start_idle;
	
	int64_t total_time;
	int64_t total_vm_time;
	int64_t total_textures_time;
	int64_t total_idle_time;

	uint8_t framecounter;               // keeps track of no of frames since last evaluation
	uint8_t framecounter_interval;      // amount of frames between two evaluations

	uint8_t status_bar_framecounter;    // the status bar at the bottom is refreshed every few frames
	uint8_t status_bar_framecounter_interval;
    
	double alpha;                       // exponential smoothing constant
	double alpha_cpu;

	double framerate;
	double smoothed_framerate;

	double audio_queue_size_bytes;
	//double smoothed_audio_queue_size_bytes;
	
	double vm_per_frame;
	double smoothed_vm_per_frame;
	double textures_per_frame;
	double smoothed_textures_per_frame;
	double idle_per_frame;
	double smoothed_idle_per_frame;
    
	char statistics_string[256];
    
public:
	stats_t(E64::host_t *h);
	
	void reset();
    
	uint32_t frametime;      // in microseconds
	
	inline void start_vm_time()
	{
		start_vm = std::chrono::steady_clock::now();
		total_idle_time += std::chrono::duration_cast<std::chrono::microseconds>(start_vm - start_idle).count();
		total_time += std::chrono::duration_cast<std::chrono::microseconds>(start_vm - start_vm_old).count();
		start_vm_old = start_vm;
	}
	
	inline void start_update_textures_time()
	{
		start_update_textures = std::chrono::steady_clock::now();
		total_vm_time += std::chrono::duration_cast<std::chrono::microseconds>(start_update_textures - start_vm).count();
	}
	
	inline void start_idle_time()
	{
		start_idle = std::chrono::steady_clock::now();
		total_textures_time += std::chrono::duration_cast<std::chrono::microseconds>(start_idle - start_update_textures).count();
	}

	// process calculations on parameters (fps/mhz/buffersize)
	void process_parameters();

	inline double current_framerate()          { return framerate; }
	inline double current_smoothed_framerate() { return smoothed_framerate; }
	inline double current_audio_queue_size()   { return audio_queue_size_bytes; }
	inline char   *summary()                   { return statistics_string; }
};

}

#endif
