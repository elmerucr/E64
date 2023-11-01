/*
 * timer.hpp
 * E64
 *
 * Copyright © 2023 elmerucr. All rights reserved.
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

namespace E64
{

class timer_t {
public:
	timer_t();
	
	void set_interval_frequency(double f);	// per second
	void set_interval_time(double t);	// in seconds
	void set_interval_bpm(uint16_t bpm);	// bpm
	
	void start_once();
	void start_repeat();
	void stop();
	
	inline bool is_activated() { return activated; }
	inline bool is_once() { return once; }
	inline bool is_repeat() { return !once; }
	
	// proceed 1 cycle
	inline bool clock() {
		if (activated) {
			if (!--counter) {
				if (once) {
					activated = false;
				} else {
					counter = clock_interval;
				}
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	
private:
	uint64_t clock_interval;
	uint64_t counter;
	void check_clock_interval();
	
	bool activated;
	bool once;
};

}

#endif
